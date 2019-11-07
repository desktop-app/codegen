add_executable(codegen_lang)
init_target(codegen_lang)

get_filename_component(src_loc ../.. REALPATH)

target_include_directories(codegen_lang
PUBLIC
    ${src_loc}
)

target_sources(codegen_lang
PRIVATE
    ${src_loc}/codegen/lang/generator.cpp
    ${src_loc}/codegen/lang/generator.h
    ${src_loc}/codegen/lang/main.cpp
    ${src_loc}/codegen/lang/options.cpp
    ${src_loc}/codegen/lang/options.h
    ${src_loc}/codegen/lang/parsed_file.cpp
    ${src_loc}/codegen/lang/parsed_file.h
    ${src_loc}/codegen/lang/processor.cpp
    ${src_loc}/codegen/lang/processor.h
)

target_link_libraries(codegen_lang
PUBLIC
    codegen_common
    external_qt
    common
)
