// This file is part of Desktop App Toolkit,
// a set of libraries for developing nice desktop applications.
//
// For license and copyright information please follow this link:
// https://github.com/desktop-app/legal/blob/master/LEGAL
//
#include "codegen/emoji/replaces.h"

#include "codegen/emoji/data.h"
#include "codegen/emoji/data_old.h"
#include <QtCore/QFile>
#include <QtCore/QJsonDocument>
#include <QtCore/QJsonObject>
#include <QtCore/QRegularExpression>

namespace codegen {
namespace emoji {
namespace {

constexpr auto kErrorBadReplaces = 402;

common::LogStream logReplacesError(const QString &filename) {
	return common::logError(kErrorBadReplaces, filename) << "Bad data: ";
}

auto RegExpCode = QRegularExpression("^:[\\+\\-a-z0-9_]+:$");
auto RegExpTone = QRegularExpression("_tone[0-9]");
auto RegExpHex = QRegularExpression("^[0-9a-f]+$");

class ReplacementWords {
public:
	ReplacementWords(const QString &string);
	QVector<QString> result() const;

private:
	QMap<QString, int> wordsWithCounts_;

};

ReplacementWords::ReplacementWords(const QString &string) {
	auto feedWord = [this](QString &word) {
		if (!word.isEmpty()) {
			++wordsWithCounts_[word];
			word.clear();
		}
	};
	// Split by all non-letters-or-numbers.
	// Leave '-' and '+' inside a word only if they're followed by a number.
	auto word = QString();
	for (auto i = string.cbegin(), e = string.cend(); i != e; ++i) {
		if (i->isLetterOrNumber()) {
			word.append(*i);
			continue;
		} else if (*i == '-' || *i == '+') {
			if (i + 1 != e && (i + 1)->isNumber()) {
				word.append(*i);
				continue;
			}
		}
		feedWord(word);
	}
	feedWord(word);
}

QVector<QString> ReplacementWords::result() const {
	auto result = QVector<QString>();
	for (auto i = wordsWithCounts_.cbegin(), e = wordsWithCounts_.cend(); i != e; ++i) {
		for (auto j = 0, count = i.value(); j != count; ++j) {
			result.push_back(i.key());
		}
	}
	return result;
}

bool AddReplacement(Replaces &result, const Id &id, const QString &replacement, const QString &name) {
	auto replace = Replace();
	replace.id = id;
	replace.replacement = replacement;
	replace.words = (ReplacementWords(replacement)).result();// + ReplacementWords(name)).result();
	if (replace.words.isEmpty()) {
		logReplacesError(result.filename) << "Child '" << replacement.toStdString() << "' has no words.";
		return false;
	}
	result.list.push_back(replace);
	return true;
}

QString ComposeString(const std::initializer_list<QChar> &chars) {
	auto result = QString();
	result.reserve(chars.size());
	for (auto ch : chars) {
		result.append(ch);
	}
	return result;
}

const auto NotSupported = [] {
	auto result = QSet<QString>();
	auto insert = [&result](auto... args) {
		result.insert(ComposeString({ args... }));
	};
	insert(0x0023, 0xFE0F); // :pound_symbol:
	insert(0x002A, 0xFE0F); // :asterisk_symbol:
	for (auto i = 0; i != 10; ++i) {
		insert(0x0030 + i, 0xFE0F); // :digit_zero: ... :digit_nine:
	}
	for (auto i = 0; i != 5; ++i) {
		insert(0xD83C, 0xDFFB + i); // :tone1: ... :tone5:
	}
	for (auto i = 0; i != 26; ++i) {
		insert(0xD83C, 0xDDE6 + i); // :regional_indicator_a: ... :regional_indicator_z:
	}
	insert(0x2640, 0xFE0F); // :female_sign:
	insert(0x2642, 0xFE0F); // :male_sign:
	insert(0x2695, 0xFE0F); // :medical_symbol:

	return result;
}();

const auto ConvertMap = ([] {
	auto result = QMap<QString, QString>();
	auto insert = [&result](const std::initializer_list<QChar> &from, const std::initializer_list<QChar> &to) {
		result.insert(ComposeString(from), ComposeString(to));
	};
	auto insertWithAdd = [&result](const std::initializer_list<QChar> &from, const QString &added) {
		auto code = ComposeString(from);
		result.insert(code, code + added);
	};
	auto maleModifier = ComposeString({ 0x200D, 0x2642, 0xFE0F });
	auto femaleModifier = ComposeString({ 0x200D, 0x2640, 0xFE0F });
	insertWithAdd({ 0xD83E, 0xDD26 }, maleModifier);
	insertWithAdd({ 0xD83E, 0xDD37 }, femaleModifier);
	insertWithAdd({ 0xD83E, 0xDD38 }, maleModifier);
	insertWithAdd({ 0xD83E, 0xDD39 }, maleModifier);
	insertWithAdd({ 0xD83E, 0xDD3C }, maleModifier);
	insertWithAdd({ 0xD83E, 0xDD3D }, maleModifier);
	insertWithAdd({ 0xD83E, 0xDD3E }, femaleModifier);

	// :kiss_woman_man:
	insert({ 0xD83D, 0xDC69, 0x200D, 0x2764, 0xFE0F, 0x200D, 0xD83D, 0xDC8B, 0x200D, 0xD83D, 0xDC68 }, { 0xD83D, 0xDC8F });

	// :family_man_woman_boy:
	insert({ 0xD83D, 0xDC68, 0x200D, 0xD83D, 0xDC69, 0x200D, 0xD83D, 0xDC66 }, { 0xD83D, 0xDC6A });

	// :couple_with_heart_woman_man:
	insert({ 0xD83D, 0xDC69, 0x200D, 0x2764, 0xFE0F, 0x200D, 0xD83D, 0xDC68 }, { 0xD83D, 0xDC91 });

	auto insertFlag = [insert](char ch1, char ch2, char ch3, char ch4) {
		insert({ 0xD83C, 0xDDE6 + (ch1 - 'a'), 0xD83C, 0xDDe6 + (ch2 - 'a') }, { 0xD83C, 0xDDE6 + (ch3 - 'a'), 0xD83C, 0xDDe6 + (ch4 - 'a') });
	};
	insertFlag('a', 'c', 's', 'h');
	insertFlag('b', 'v', 'n', 'o');
	insertFlag('c', 'p', 'f', 'r');
	insertFlag('d', 'g', 'i', 'o');
	insertFlag('e', 'a', 'e', 's');
	insertFlag('h', 'm', 'a', 'u');
	insertFlag('m', 'f', 'f', 'r');
	insertFlag('s', 'j', 'n', 'o');
	insertFlag('t', 'a', 's', 'h');
	insertFlag('u', 'm', 'u', 's');

	return result;
})();

// Empty string result means we should skip this one.
QString ConvertEmojiId(const Id &id, const QString &replacement) {
	if (RegExpTone.match(replacement).hasMatch()) {
		return QString();
	}
	if (NotSupported.contains(id)) {
		return QString();
	}
	return ConvertMap.value(id, id);
}

} // namespace

Replaces PrepareReplaces(const QString &filename) {
	auto result = Replaces(filename);
	auto content = ([filename] {
		QFile f(filename);
		return f.open(QIODevice::ReadOnly) ? f.readAll() : QByteArray();
	})();
	if (content.isEmpty()) {
		logReplacesError(filename) << "Could not read data.";
		return result;
	}
	auto error = QJsonParseError();
	auto document = QJsonDocument::fromJson(content, &error);
	if (error.error != QJsonParseError::NoError) {
		logReplacesError(filename) << "Could not parse data (" << int(error.error) << "): " << error.errorString().toStdString();
		return result;
	}
	if (!document.isObject()) {
		logReplacesError(filename) << "Root object not found.";
		return result;
	}
	auto list = document.object();
	for (auto i = list.constBegin(), e = list.constEnd(); i != e; ++i) {
		if (!(*i).isObject()) {
			logReplacesError(filename) << "Child object not found.";
			return Replaces(filename);
		}
		auto childKey = i.key();
		auto child = (*i).toObject();
		auto failed = false;
		auto getString = [filename, childKey, &child, &failed](const QString &key) {
			auto it = child.constFind(key);
			if (it == child.constEnd() || !(*it).isString()) {
				logReplacesError(filename) << "Child '" << childKey.toStdString() << "' field not found: " << key.toStdString();
				failed = true;
				return QString();
			}
			return (*it).toString();
		};
		auto idParts = getString("output").split('-');
		auto name = getString("name");
		auto replacement = getString("alpha_code");
		auto aliases = getString("aliases").split('|');
		const auto Exceptions = { ":shrug:" };
		for (const auto &exception : Exceptions) {
			const auto index = aliases.indexOf(exception);
			if (index >= 0) {
				aliases.removeAt(index);
			}
		}
		if (aliases.size() == 1 && aliases[0].isEmpty()) {
			aliases.clear();
		}
		if (failed) {
			return Replaces(filename);
		}
		if (!RegExpCode.match(replacement).hasMatch()) {
			logReplacesError(filename) << "Child '" << childKey.toStdString() << "' alpha_code invalid: " << replacement.toStdString();
			return Replaces(filename);
		}
		for (auto &alias : aliases) {
			if (!RegExpCode.match(alias).hasMatch()) {
				logReplacesError(filename) << "Child '" << childKey.toStdString() << "' alias invalid: " << alias.toStdString();
				return Replaces(filename);
			}
		}
		auto id = Id();
		for (auto &idPart : idParts) {
			auto ok = true;
			auto utf32 = idPart.toInt(&ok, 0x10);
			if (!ok || !RegExpHex.match(idPart).hasMatch()) {
				logReplacesError(filename) << "Child '" << childKey.toStdString() << "' output part invalid: " << idPart.toStdString();
				return Replaces(filename);
			}
			if (utf32 >= 0 && utf32 < 0x10000) {
				auto ch = QChar(ushort(utf32));
				if (ch.isLowSurrogate() || ch.isHighSurrogate()) {
					logReplacesError(filename) << "Child '" << childKey.toStdString() << "' output part invalid: " << idPart.toStdString();
					return Replaces(filename);
				}
				id.append(ch);
			} else if (utf32 >= 0x10000 && utf32 <= 0x10FFFF) {
				auto hi = ((utf32 - 0x10000) / 0x400) + 0xD800;
				auto lo = ((utf32 - 0x10000) % 0x400) + 0xDC00;
				id.append(QChar(ushort(hi)));
				id.append(QChar(ushort(lo)));
			} else {
				logReplacesError(filename) << "Child '" << childKey.toStdString() << "' output part invalid: " << idPart.toStdString();
				return Replaces(filename);
			}
		}
		id = ConvertEmojiId(id, replacement);
		if (id.isEmpty()) {
			continue;
		}
		if (!AddReplacement(result, id, replacement, name)) {
			return Replaces(filename);
		}
		for (auto &alias : aliases) {
			if (!AddReplacement(result, id, alias, name)) {
				return Replaces(filename);
			}
		}
	}
	if (!AddReplacement(result, ComposeString({ 0xD83D, 0xDC4D }), ":like:", "thumbs up")) {
		return Replaces(filename);
	}
	if (!AddReplacement(result, ComposeString({ 0xD83D, 0xDC4E }), ":dislike:", "thumbs down")) {
		return Replaces(filename);
	}
	if (!AddReplacement(result, ComposeString({ 0xD83E, 0xDD14 }), ":hmm:", "thinking")) {
		return Replaces(filename);
	}
	if (!AddReplacement(result, ComposeString({ 0xD83E, 0xDD73 }), ":party:", "celebration")) {
		return Replaces(filename);
	}
	return result;
}

bool CheckAndConvertReplaces(Replaces &replaces, const Data &data) {
	auto result = Replaces(replaces.filename);
	auto sorted = QMultiMap<Id, Replace>();
	auto findId = [&](const Id &id) {
		return data.map.find(id) != data.map.cend();
	};
	auto findAndSort = [&](Id id, const Replace &replace) {
		if (!findId(id)) {
			id.replace(QChar(0xFE0F), QString());
			if (!findId(id)) {
				return false;
			}
		}
		auto it = data.map.find(id);
		id = data.list[it->second].id;
		if (data.list[it->second].postfixed) {
			id += QChar(kPostfix);
		}
		auto inserted = sorted.insert(id, replace);
		inserted.value().id = id;
		return true;
	};

	auto success = true;

	// Find all replaces in data.map, adjust id if necessary.
	// Store all replaces in sorted map to find them fast afterwards.
	auto maleModifier = ComposeString({ 0x200D, 0x2642, 0xFE0F });
	auto femaleModifier = ComposeString({ 0x200D, 0x2640, 0xFE0F });
	for (auto &replace : replaces.list) {
		if (findAndSort(replace.id, replace)) {
			continue;
		}
		if (replace.id.endsWith(maleModifier)) {
			auto defaultId = replace.id.mid(0, replace.id.size() - maleModifier.size());
			if (findAndSort(defaultId, replace)) {
				continue;
			}
		} else if (replace.id.endsWith(femaleModifier)) {
			auto defaultId = replace.id.mid(0, replace.id.size() - femaleModifier.size());
			if (findAndSort(defaultId, replace)) {
				continue;
			}
		} else if (findId(replace.id + maleModifier)) {
			if (findId(replace.id + femaleModifier)) {
				logReplacesError(replaces.filename)
					<< "Replace '"
					<< replace.replacement.toStdString()
					<< "' ("
					<< replace.id.toStdString()
					<< ") ambiguous.";
				success = false;
				continue;
			} else {
				findAndSort(replace.id + maleModifier, replace);
				continue;
			}
		} else if (findAndSort(replace.id + femaleModifier, replace)) {
			continue;
		}
		logReplacesError(replaces.filename)
			<< "Replace '"
			<< replace.replacement.toStdString()
			<< "' ("
			<< replace.id.toStdString()
			<< ") not found.";
		success = false;
		continue;
	}

	// Go through all categories and put all replaces in order of emoji in categories.
	result.list.reserve(replaces.list.size());
	for (auto &category : data.categories) {
		for (auto index : category) {
			auto id = data.list[index].id;
			if (data.list[index].postfixed) {
				id += QChar(kPostfix);
			}
			for (auto it = sorted.find(id); it != sorted.cend(); sorted.erase(it), it = sorted.find(id)) {
				result.list.push_back(it.value());
			}
		}
	}
	if (result.list.size() != replaces.list.size()) {
		logReplacesError(replaces.filename) << "Some were not found.";
		success = false;
	}
	if (!sorted.isEmpty()) {
		logReplacesError(replaces.filename) << "Weird.";
		success = false;
	}
	if (!success) {
		return false;
	}
	replaces = std::move(result);
	return true;
}

} // namespace emoji
} // namespace codegen
