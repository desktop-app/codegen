add_library(codegen_common OBJECT)
init_target(codegen_common)

get_filename_component(src_loc ../.. REALPATH)

target_include_directories(codegen_common
PUBLIC
    ${src_loc}
)

target_sources(codegen_common
PRIVATE
    ${src_loc}/codegen/common/basic_tokenized_file.cpp
    ${src_loc}/codegen/common/basic_tokenized_file.h
    ${src_loc}/codegen/common/checked_utf8_string.cpp
    ${src_loc}/codegen/common/checked_utf8_string.h
    ${src_loc}/codegen/common/clean_file.cpp
    ${src_loc}/codegen/common/clean_file.h
    ${src_loc}/codegen/common/clean_file_reader.h
    ${src_loc}/codegen/common/const_utf8_string.h
    ${src_loc}/codegen/common/cpp_file.cpp
    ${src_loc}/codegen/common/cpp_file.h
    ${src_loc}/codegen/common/logging.cpp
    ${src_loc}/codegen/common/logging.h
)
target_link_libraries(codegen_common
PUBLIC
    external_qt
)
