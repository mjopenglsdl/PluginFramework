#
set(CMAKE_EXE_LINKER_FLAGS "-rdynamic")
include_directories(${CMAKE_SOURCE_DIR}/include)

add_library(Bar MODULE Bar.cpp)
target_compile_features(Bar PRIVATE cxx_variadic_templates)

add_library(Foo MODULE Foo.cpp)
target_compile_features(Foo PRIVATE cxx_variadic_templates)

add_executable(baseTest main.cpp)
target_link_libraries(baseTest dl)
target_compile_features(baseTest PRIVATE cxx_variadic_templates)
