// This file is part of Desktop App Toolkit,
// a set of libraries for developing nice desktop applications.
//
// For license and copyright information please follow this link:
// https://github.com/desktop-app/legal/blob/master/LEGAL
//
#pragma once

#include "codegen/emoji/data_old.h"

#include <set>

namespace codegen {
namespace emoji {

[[nodiscard]] InputId InputIdFromString(const QString &emoji);
[[nodiscard]] QString InputIdToString(const InputId &id);
[[nodiscard]] InputData ReadData(
	const QString &path,
	std::set<QString> *allEmoji = nullptr);

} // namespace emoji
} // namespace codegen
