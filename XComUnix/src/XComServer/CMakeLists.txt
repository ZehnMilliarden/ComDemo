aux_source_directory(./ SRC_LIST)
message(${SRC_LIST})

add_library(XComServer SHARED ${SRC_LIST})
set_target_properties(XComServer PROPERTIES PREFIX "")

target_include_directories(XComServer PRIVATE ${PROJECT_SOURCE_DIR})
target_include_directories(XComServer PRIVATE ${PROJECT_SOURCE_DIR}/src)

target_compile_options(XComServer PRIVATE -fPIC -Wall -g -fvisibility=hidden)
install(TARGETS XComServer DESTINATION ${PROJECT_SOURCE_DIR}/bin)