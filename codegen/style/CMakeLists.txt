add_executable(codegen_style)
init_target(codegen_style)

get_filename_component(src_loc ../.. REALPATH)

target_include_directories(codegen_style
PUBLIC
    ${src_loc}
)

target_sources(codegen_style
PRIVATE
    ${src_loc}/codegen/style/generator.cpp
    ${src_loc}/codegen/style/generator.h
    ${src_loc}/codegen/style/main.cpp
    ${src_loc}/codegen/style/module.cpp
    ${src_loc}/codegen/style/module.h
    ${src_loc}/codegen/style/options.cpp
    ${src_loc}/codegen/style/options.h
    ${src_loc}/codegen/style/parsed_file.cpp
    ${src_loc}/codegen/style/parsed_file.h
    ${src_loc}/codegen/style/processor.cpp
    ${src_loc}/codegen/style/processor.h
    ${src_loc}/codegen/style/structure_types.cpp
    ${src_loc}/codegen/style/structure_types.h
)

target_link_libraries(codegen_style
PUBLIC
    codegen_common
    lib_base
    lib_crl
    common
)
