add_executable(codegen_numbers)
init_target(codegen_numbers)

get_filename_component(src_loc ../.. REALPATH)

set(codegen_numbers_sources
PRIVATE
    codegen/numbers/generator.cpp
    codegen/numbers/generator.h
    codegen/numbers/main.cpp
    codegen/numbers/options.cpp
    codegen/numbers/options.h
    codegen/numbers/parsed_file.cpp
    codegen/numbers/parsed_file.h
    codegen/numbers/processor.cpp
    codegen/numbers/processor.h
)
nice_target_sources(codegen_numbers ${src_loc} "${codegen_numbers_sources}")

target_include_directories(codegen_numbers
PUBLIC
    ${src_loc}
)

target_link_libraries(codegen_numbers
PUBLIC
    codegen_common
    external_qt
    common
)
