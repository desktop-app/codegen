// This file is part of Desktop App Toolkit,
// a set of libraries for developing nice desktop applications.
//
// For license and copyright information please follow this link:
// https://github.com/desktop-app/legal/blob/master/LEGAL
//
#include "codegen/style/processor.h"

#include <QtCore/QDir>
#include <QtCore/QFile>
#include <QtCore/QFileInfo>
#include "codegen/common/cpp_file.h"
#include "codegen/style/parsed_file.h"
#include "codegen/style/generator.h"

namespace codegen {
namespace style {
namespace {

constexpr int kErrorCantWritePath = 821;

QString destFileBaseName(const structure::Module &module) {
	return "style_" + QFileInfo(module.filepath()).baseName();
}

} // namespace

Processor::Processor(const Options &options)
: options_(options) {
}

int Processor::launch() {
	auto cache = std::map<QString, std::shared_ptr<const structure::Module>>();
	for (auto i = 0; i != options_.inputPaths.size(); ++i) {
		auto parser = ParsedFile(cache, options_, i);
		if (!parser.read()) {
			return -1;
		}

		const auto module = parser.getResult();
		if (!write(*module)) {
			return -1;
		}
	}
	if (!writeAggregator()) {
		return -1;
	}
	if (!common::TouchTimestamp(options_.timestampPath)) {
		return -1;
	}
	return 0;
}

bool Processor::write(const structure::Module &module) {
	bool forceReGenerate = false;
	QDir dir(options_.outputPath);
	if (!dir.mkpath(".")) {
		common::logError(kErrorCantWritePath, "Command Line") << "can not open path for writing: " << dir.absolutePath().toStdString();
		return false;
	}

	QFileInfo srcFile(module.filepath());
	QString dstFilePath = dir.absolutePath() + '/' + (options_.isPalette ? "palette" : destFileBaseName(module));

	common::ProjectInfo project = {
		"codegen_style",
		srcFile.fileName(),
		forceReGenerate
	};

	Generator generator(module, dstFilePath, project, options_.isPalette);
	if (!generator.writeHeader()
		|| !generator.writeSource()
		|| !generator.writeMasksHeader()
		|| !generator.writeMasksSource()
		|| !generator.writeNewHeader()
		|| !generator.writeNewSource()) {
		return false;
	}
	if (!options_.isPalette) {
		moduleBaseNames_.push_back(destFileBaseName(module));
	}
	return true;
}

bool Processor::writeAggregator() const {
	if (options_.packageName.isEmpty()) {
		return true;
	}

	const auto &package = options_.packageName;
	QDir dir(options_.outputPath);
	const auto basePath = dir.absolutePath() + '/' + package + "_modules";

	common::ProjectInfo project = {
		"codegen_style",
		QStringLiteral("modules aggregator"),
		false
	};

	{
		auto header = std::make_unique<common::CppFile>(
			basePath + ".h",
			project);
		header->include("ui/style/style_runtime.h").newline();
		header->pushNamespace("style").pushNamespace(package);
		header->newline();
		header->stream()
			<< "void RegisterModules(style::Registry &registry);\n";
		header->newline();
		if (!header->finalize()) return false;
	}

	auto source = std::make_unique<common::CppFile>(
		basePath + ".cpp",
		project);
	const auto kStylePrefix = QStringLiteral("style_");
	for (const auto &base : moduleBaseNames_) {
		const auto pureBase = base.startsWith(kStylePrefix)
			? base.mid(kStylePrefix.size())
			: base;
		source->include("styles/" + pureBase + ".h");
	}
	source->newline();
	source->pushNamespace("style").pushNamespace(package);
	source->newline();
	source->stream()
		<< "void RegisterModules(style::Registry &registry) {\n";
	if (moduleBaseNames_.isEmpty()) {
		source->stream() << "\t(void)registry;\n";
	} else {
		source->stream() << "\tstatic const style::ModuleDescriptor *const kDescriptors[] = {\n";
		for (const auto &base : moduleBaseNames_) {
			source->stream()
				<< "\t\t&::style::modules::" << base << "::Descriptor,\n";
		}
		source->stream() << "\t};\n";
		source->stream()
			<< "\tregistry.add(kDescriptors, "
			<< moduleBaseNames_.size() << ");\n";
	}
	source->stream() << "}\n";
	return source->finalize();
}

Processor::~Processor() = default;

} // namespace style
} // namespace codegen
