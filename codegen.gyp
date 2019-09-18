# This file is part of Desktop App Toolkit,
# a set of libraries for developing nice desktop applications.
#
# For license and copyright information please follow this link:
# https://github.com/desktop-app/legal/blob/master/LEGAL

{
  'includes': [
    '../gyp/helpers/common/common.gypi',
  ],
  'variables': {
    'src_loc': '.',
    'codegen_src_loc': '<(src_loc)/codegen',
    'mac_target': '10.10',
  },
  'targets': [{
    'target_name': 'codegen_lang',
    'includes': [
      '../gyp/helpers/common/executable.gypi',
      '../gyp/helpers/modules/qt.gypi',
    ],

    'include_dirs': [
      '<(src_loc)',
    ],
    'sources': [
      '<(codegen_src_loc)/common/basic_tokenized_file.cpp',
      '<(codegen_src_loc)/common/basic_tokenized_file.h',
      '<(codegen_src_loc)/common/checked_utf8_string.cpp',
      '<(codegen_src_loc)/common/checked_utf8_string.h',
      '<(codegen_src_loc)/common/clean_file.cpp',
      '<(codegen_src_loc)/common/clean_file.h',
      '<(codegen_src_loc)/common/clean_file_reader.h',
      '<(codegen_src_loc)/common/const_utf8_string.h',
      '<(codegen_src_loc)/common/cpp_file.cpp',
      '<(codegen_src_loc)/common/cpp_file.h',
      '<(codegen_src_loc)/common/logging.cpp',
      '<(codegen_src_loc)/common/logging.h',
      '<(codegen_src_loc)/lang/generator.cpp',
      '<(codegen_src_loc)/lang/generator.h',
      '<(codegen_src_loc)/lang/main.cpp',
      '<(codegen_src_loc)/lang/options.cpp',
      '<(codegen_src_loc)/lang/options.h',
      '<(codegen_src_loc)/lang/parsed_file.cpp',
      '<(codegen_src_loc)/lang/parsed_file.h',
      '<(codegen_src_loc)/lang/processor.cpp',
      '<(codegen_src_loc)/lang/processor.h',
    ],
  }, {
    'target_name': 'codegen_style',
    'includes': [
      '../gyp/helpers/common/executable.gypi',
      '../gyp/helpers/modules/qt.gypi',
    ],
    'dependencies': [
      '<(submodules_loc)/lib_base/lib_base.gyp:lib_base',
    ],
    'include_dirs': [
      '<(src_loc)',
    ],
    'sources': [
      '<(codegen_src_loc)/common/basic_tokenized_file.cpp',
      '<(codegen_src_loc)/common/basic_tokenized_file.h',
      '<(codegen_src_loc)/common/checked_utf8_string.cpp',
      '<(codegen_src_loc)/common/checked_utf8_string.h',
      '<(codegen_src_loc)/common/clean_file.cpp',
      '<(codegen_src_loc)/common/clean_file.h',
      '<(codegen_src_loc)/common/clean_file_reader.h',
      '<(codegen_src_loc)/common/const_utf8_string.h',
      '<(codegen_src_loc)/common/cpp_file.cpp',
      '<(codegen_src_loc)/common/cpp_file.h',
      '<(codegen_src_loc)/common/logging.cpp',
      '<(codegen_src_loc)/common/logging.h',
      '<(codegen_src_loc)/style/generator.cpp',
      '<(codegen_src_loc)/style/generator.h',
      '<(codegen_src_loc)/style/main.cpp',
      '<(codegen_src_loc)/style/module.cpp',
      '<(codegen_src_loc)/style/module.h',
      '<(codegen_src_loc)/style/options.cpp',
      '<(codegen_src_loc)/style/options.h',
      '<(codegen_src_loc)/style/parsed_file.cpp',
      '<(codegen_src_loc)/style/parsed_file.h',
      '<(codegen_src_loc)/style/processor.cpp',
      '<(codegen_src_loc)/style/processor.h',
      '<(codegen_src_loc)/style/structure_types.cpp',
      '<(codegen_src_loc)/style/structure_types.h',
    ],
  }, {
    'target_name': 'codegen_numbers',
    'includes': [
      '../gyp/helpers/common/executable.gypi',
      '../gyp/helpers/modules/qt.gypi',
    ],

    'include_dirs': [
      '<(src_loc)',
    ],
    'sources': [
      '<(codegen_src_loc)/common/basic_tokenized_file.cpp',
      '<(codegen_src_loc)/common/basic_tokenized_file.h',
      '<(codegen_src_loc)/common/checked_utf8_string.cpp',
      '<(codegen_src_loc)/common/checked_utf8_string.h',
      '<(codegen_src_loc)/common/clean_file.cpp',
      '<(codegen_src_loc)/common/clean_file.h',
      '<(codegen_src_loc)/common/clean_file_reader.h',
      '<(codegen_src_loc)/common/const_utf8_string.h',
      '<(codegen_src_loc)/common/cpp_file.cpp',
      '<(codegen_src_loc)/common/cpp_file.h',
      '<(codegen_src_loc)/common/logging.cpp',
      '<(codegen_src_loc)/common/logging.h',
      '<(codegen_src_loc)/numbers/generator.cpp',
      '<(codegen_src_loc)/numbers/generator.h',
      '<(codegen_src_loc)/numbers/main.cpp',
      '<(codegen_src_loc)/numbers/options.cpp',
      '<(codegen_src_loc)/numbers/options.h',
      '<(codegen_src_loc)/numbers/parsed_file.cpp',
      '<(codegen_src_loc)/numbers/parsed_file.h',
      '<(codegen_src_loc)/numbers/processor.cpp',
      '<(codegen_src_loc)/numbers/processor.h',
    ],
  }, {
    'target_name': 'codegen_emoji',
    'includes': [
      '../gyp/helpers/common/executable.gypi',
      '../gyp/helpers/modules/qt.gypi',
    ],

    'include_dirs': [
      '<(src_loc)',
    ],
    'sources': [
      '<(codegen_src_loc)/common/cpp_file.cpp',
      '<(codegen_src_loc)/common/cpp_file.h',
      '<(codegen_src_loc)/common/logging.cpp',
      '<(codegen_src_loc)/common/logging.h',
      '<(codegen_src_loc)/emoji/data.cpp',
      '<(codegen_src_loc)/emoji/data.h',
      '<(codegen_src_loc)/emoji/generator.cpp',
      '<(codegen_src_loc)/emoji/generator.h',
      '<(codegen_src_loc)/emoji/main.cpp',
      '<(codegen_src_loc)/emoji/options.cpp',
      '<(codegen_src_loc)/emoji/options.h',
      '<(codegen_src_loc)/emoji/replaces.cpp',
      '<(codegen_src_loc)/emoji/replaces.h',
    ],
  }],
}
