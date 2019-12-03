add_executable(codegen_numbers)
init_target(codegen_numbers "(codegen)")

get_filename_component(src_loc ../.. REALPATH)

nice_target_sources(codegen_numbers ${src_loc}
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

target_include_directories(codegen_numbers
PUBLIC
    ${src_loc}
)

target_link_libraries(codegen_numbers
PUBLIC
    desktop-app::codegen_common
    desktop-app::external_qt
)
