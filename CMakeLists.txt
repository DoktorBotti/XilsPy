cmake_minimum_required(VERSION 3.4...3.18)
project(xisl_binding)

add_subdirectory(pybind11)

# make xisl visible here
add_library(xisl SHARED IMPORTED)
set_target_properties(xisl PROPERTIES
        IMPORTED_LOCATION "${CMAKE_CURRENT_SOURCE_DIR}/xislLib/XISL.dll"
        IMPORTED_IMPLIB "${CMAKE_CURRENT_SOURCE_DIR}/xislLib/XISL.lib"
        INTERFACE_INCLUDE_DIRECTORIES "${CMAKE_CURRENT_SOURCE_DIR}/xislLib/include"
        )
set_target_properties(xisl PROPERTIES LINKER_LANGUAGE C)
# find python version to create a binding to (version can be set in line below)
find_package(PythonLibs REQUIRED)

# compile standalone test
add_executable(standalone_test ${CMAKE_CURRENT_SOURCE_DIR}/src/standalone_main.cpp)
target_link_libraries(standalone_test xisl)
# copies xisl.dll next to standalone_test.exe
add_custom_command(TARGET standalone_test POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy
        ${CMAKE_CURRENT_SOURCE_DIR}/xislLib/XISL.dll $<TARGET_FILE_DIR:standalone_test>/xisl.dll)

# create static and private (underscored) library target containing xisl and its bindings
add_library(_binding_compilation STATIC ${CMAKE_CURRENT_SOURCE_DIR}/src/bindings.cpp)
target_link_libraries(_binding_compilation ${PYTHON_LIBRARIES} xisl)
set_target_properties(_binding_compilation PROPERTIES POSITION_INDEPENDENT_CODE ON)
target_include_directories(_binding_compilation PRIVATE
        ${PROJECT_SOURCE_DIR}/pybind11/include
        ${PYTHON_INCLUDE_DIRS}
        )

# create pybind module from private target
pybind11_add_module(xisl_binding MODULE ${CMAKE_CURRENT_SOURCE_DIR}/src/bindings.cpp)
target_link_libraries(xisl_binding PRIVATE _binding_compilation)

# EXAMPLE_VERSION_INFO is defined by setup.py and passed into the C++ code as a
# define (VERSION_INFO) here.
target_compile_definitions(xisl_binding
                           PRIVATE VERSION_INFO=${EXAMPLE_VERSION_INFO})
