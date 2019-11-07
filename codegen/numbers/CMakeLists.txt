add_executable(codegen_numbers)
init_target(codegen_numbers)

get_filename_component(src_loc ../.. REALPATH)

target_include_directories(codegen_numbers
PUBLIC
    ${src_loc}
)

target_sources(codegen_numbers
PRIVATE
    ${src_loc}/codegen/numbers/generator.cpp
    ${src_loc}/codegen/numbers/generator.h
    ${src_loc}/codegen/numbers/main.cpp
    ${src_loc}/codegen/numbers/options.cpp
    ${src_loc}/codegen/numbers/options.h
    ${src_loc}/codegen/numbers/parsed_file.cpp
    ${src_loc}/codegen/numbers/parsed_file.h
    ${src_loc}/codegen/numbers/processor.cpp
    ${src_loc}/codegen/numbers/processor.h
)

target_link_libraries(codegen_numbers
PUBLIC
    codegen_common
    external_qt
    common
)
