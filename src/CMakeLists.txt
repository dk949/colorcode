link_compile_commands()

file(GLOB_RECURSE SOURCE_FILES "${CMAKE_SOURCE_DIR}/src/*.cpp")
file(GLOB_RECURSE HEADER_FILES "${CMAKE_SOURCE_DIR}/src/*.hpp")

add_executable(${EXE_NAME} ${SOURCE_FILES} ${HEADER_FILES})

target_include_directories(
    ${EXE_NAME} PRIVATE "${CMAKE_BINARY_DIR}/project_config/include"
)

find_package(fmt CONFIG REQUIRED)
target_link_libraries(${EXE_NAME} PRIVATE fmt::fmt)

target_link_libraries(${EXE_NAME} PRIVATE pcre2-8)

enable_sanitizers(${EXE_NAME})
set_target_warnings(${EXE_NAME})
target_compile_definitions(${EXE_NAME} PRIVATE PCRE2_CODE_UNIT_WIDTH=8)
