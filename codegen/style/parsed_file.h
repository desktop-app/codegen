// This file is part of Desktop App Toolkit,
// a set of libraries for developing nice desktop applications.
//
// For license and copyright information please follow this link:
// https://github.com/desktop-app/legal/blob/master/LEGAL
//
#pragma once

#include <memory>
#include <string>
#include <functional>
#include <QImage>
#include "codegen/common/basic_tokenized_file.h"
#include "codegen/style/options.h"
#include "codegen/style/module.h"

namespace codegen {
namespace style {

using Modifier = std::function<void(QImage &image)>;
Modifier GetModifier(const QString &name);

[[nodiscard]] std::optional<QSize> GetSizeModifier(const QString &value);

// Parses an input file to the internal struct.
class ParsedFile {
public:
	ParsedFile(
		std::map<QString, std::shared_ptr<const structure::Module>> &includeCache,
		const Options &options,
		int index = 0,
		std::vector<QString> includeStack = {});
	ParsedFile(const ParsedFile &other) = delete;
	ParsedFile &operator=(const ParsedFile &other) = delete;

	bool read();

	std::unique_ptr<const structure::Module> getResult() {
		return std::move(module_);
	}

private:
	bool failed() const {
		return failed_ || file_.failed();
	}

	// Log error to std::cerr with 'code' at the current position in file.
	common::LogStream logError(int code) {
		failed_ = true;
		return file_.logError(code);
	}
	common::LogStream logErrorUnexpectedToken() {
		failed_ = true;
		return file_.logErrorUnexpectedToken();
	}
	common::LogStream logErrorTypeMismatch();
	common::LogStream logAssert(bool assertion) {
		if (!assertion) {
			return logError(common::kErrorInternal) << "internal - ";
		}
		return common::LogStream(common::LogStream::Null);
	}

	// Helper methods for context-dependent reading.
	std::shared_ptr<const structure::Module> readIncluded();
	structure::Struct readStruct(const QString &name);
	structure::Variable readVariable(const QString &name);

	structure::StructField readStructField(const QString &name);
	structure::Type readType();
	structure::Value readValue();

	structure::Value readStructValue();
	structure::Value defaultConstructedStruct(const structure::FullName &name);
	void applyStructParent(structure::Value &result, const structure::FullName &parentName);
	bool readStructValueInner(structure::Value &result);
	bool checkNoImplicitUnnamedIcons(const structure::Value &value);
	bool assignStructField(structure::Value &result, const structure::Variable &field);
	bool readStructParents(structure::Value &result);

	// Simple methods for reading value types.
	structure::Value readPositiveValue();
	structure::Value readNumericValue();
	structure::Value readStringValue();
	structure::Value readColorValue();
	structure::Value readPointValue();
	structure::Value readSizeValue();
	structure::Value readBoolValue();
	structure::Value readAlignValue();
	structure::Value readMarginsValue();
	structure::Value readFontValue();
	structure::Value readIconValue();
	structure::Value readCopyValue();

	structure::Value readNumericOrNumericCopyValue();
	structure::Value readStringOrStringCopyValue();

	structure::data::monoicon readMonoIconFields();
	QString readMonoIconFilename();

	// Read next token and fire unexpected token error if it is not of "type".
	using BasicToken = common::BasicTokenizedFile::Token;
	BasicToken assertNextToken(BasicToken::Type type);

	// Look through include directories in options_ and find absolute include path.
	Options includedOptions(const QString &filepath);

	// Compose context-dependent full name.
	structure::FullName composeFullName(const QString &name);

	std::map<QString, std::shared_ptr<const structure::Module>> includeCache_;

	QString filePath_;
	common::BasicTokenizedFile file_;
	Options options_;
	bool failed_ = false;
	std::unique_ptr<structure::Module> module_;

	std::vector<QString> includeStack_;

	QMap<std::string, structure::Type> typeNames_ = {
		{ "int"       , { structure::TypeTag::Int } },
		{ "bool"      , { structure::TypeTag::Bool } },
		{ "double"    , { structure::TypeTag::Double } },
		{ "pixels"    , { structure::TypeTag::Pixels } },
		{ "string"    , { structure::TypeTag::String } },
		{ "color"     , { structure::TypeTag::Color } },
		{ "point"     , { structure::TypeTag::Point } },
		{ "size"      , { structure::TypeTag::Size } },
		{ "align"     , { structure::TypeTag::Align } },
		{ "margins"   , { structure::TypeTag::Margins } },
		{ "font"      , { structure::TypeTag::Font } },
		{ "icon"      , { structure::TypeTag::Icon } },
	};

};

} // namespace style
} // namespace codegen
