// This file is part of Desktop App Toolkit,
// a set of libraries for developing nice desktop applications.
//
// For license and copyright information please follow this link:
// https://github.com/desktop-app/legal/blob/master/LEGAL
//
#pragma once

#include <memory>
#include <QtCore/QString>
#include <QtCore/QSet>
#include <QtGui/QImage>
#include "codegen/common/cpp_file.h"
#include "codegen/emoji/options.h"
#include "codegen/emoji/data.h"
#include "codegen/emoji/replaces.h"

namespace codegen {
namespace emoji {

using uint32 = unsigned int;

class Generator {
public:
	Generator(const Options &options);
	Generator(const Generator &other) = delete;
	Generator &operator=(const Generator &other) = delete;

	int generate();

private:
	[[nodiscard]] QImage generateImage(int imageIndex);
	bool writeImages();

	bool writeSource();
	bool writeHeader();
	bool writeSuggestionsSource();
	bool writeSuggestionsHeader();

	template <typename Callback>
	bool enumerateWholeList(Callback callback);

	bool writeInitCode();
	bool writeSections();
	bool writeReplacements();
	bool writeGetSections();
	bool writeFindReplace();
	bool writeFind();
	bool writeFindFromDictionary(
		const std::map<QString, int, std::greater<QString>> &dictionary,
		bool skipPostfixes = false,
		const std::set<int> &postfixRequired = {});
	bool writeGetReplacements();
	void startBinary();
	bool writeStringBinary(common::CppFile *source, const QString &string);
	void writeIntBinary(common::CppFile *source, int data);
	void writeUintBinary(common::CppFile *source, uint32 data);

	const common::ProjectInfo &project_;
	int colorsCount_ = 0;
	QString writeImages_;
	QString outputPath_;
	QString spritePath_;
	std::unique_ptr<common::CppFile> source_;
	Data data_;

	QString suggestionsPath_;
	std::unique_ptr<common::CppFile> suggestionsSource_;
	Replaces replaces_;

	int _binaryFullLength = 0;
	int _binaryCount = 0;

};

} // namespace emoji
} // namespace codegen
