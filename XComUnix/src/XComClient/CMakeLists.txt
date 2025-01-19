aux_source_directory(./ SRC_LIST)
message(${SRC_LIST})

add_executable(XComClient ${SRC_LIST})

target_include_directories(XComClient PRIVATE ${PROJECT_SOURCE_DIR})
target_include_directories(XComClient PRIVATE ${PROJECT_SOURCE_DIR}/src)

target_link_libraries(XComClient dl)

target_compile_options(XComClient PRIVATE -Wall -Bsymbolic -g -fvisibility=hidden)
install(TARGETS XComClient DESTINATION ${PROJECT_SOURCE_DIR}/bin)