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
#include "codegen/common/cpp_file.h"
#include "codegen/numbers/parsed_file.h"

namespace codegen {
namespace numbers {

class Generator {
public:
	Generator(const Rules &rules, const QString &destBasePath, const common::ProjectInfo &project);
	Generator(const Generator &other) = delete;
	Generator &operator=(const Generator &other) = delete;

	bool writeHeader();
	bool writeSource();

private:
	const Rules &rules_;
	QString basePath_;
	const common::ProjectInfo &project_;
	std::unique_ptr<common::CppFile> source_, header_;

};

} // namespace numbers
} // namespace codegen
