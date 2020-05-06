// This file is part of Desktop App Toolkit,
// a set of libraries for developing nice desktop applications.
//
// For license and copyright information please follow this link:
// https://github.com/desktop-app/legal/blob/master/LEGAL
//
#pragma once

#include <QtCore/QString>
#include <QtCore/QStringList>

#ifdef __APPLE__
#define SUPPORT_IMAGE_GENERATION
#endif // __APPLE__

namespace codegen {
namespace emoji {

struct Options {
	QString outputPath = ".";
	QString dataPath;
	QString replacesPath;
#ifdef SUPPORT_IMAGE_GENERATION
	bool writeImages = false;
#endif // SUPPORT_IMAGE_GENERATION
};

// Parsing failed if inputPath is empty in the result.
Options parseOptions();

} // namespace emoji
} // namespace codegen
