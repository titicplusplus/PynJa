cmake_minimum_required(VERSION 3.13)  # CMake version check

project(PynJa VERSION 1.0 DESCRIPTION "A Template Engine with Python")               # Create project "simple_example"
set(CMAKE_CXX_STANDARD 17)

add_library(PynJa SHARED
	pynja.cpp
	pynja.hpp
)

set(public_headers
	pynja.hpp
)

set_target_properties(${PROJECT_NAME} PROPERTIES VERSION ${PROJECT_VERSION})
set_target_properties(${PROJECT_NAME} PROPERTIES SOVERSION ${PROJECT_VERSION_MAJOR})

find_package(PythonLibs REQUIRED)

include_directories(
	PynJa
	PRIVATE
		${PYTHON_INCLUDE_DIRS}
)

target_link_libraries(
	PynJa	
	${PYTHON_LIBRARIES}
)

#target_compile_features(PynJa PRIVATE cxx_std_20)

set_target_properties(${PROJECT_NAME} PROPERTIES PUBLIC_HEADER "${public_headers}")

# For access to standard installation directory variables (CMAKE_INSTALL_xDIR).
include(GNUInstallDirs)

# Set library shared object and API header file to install.
install(TARGETS ${PROJECT_NAME}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/TitiCplusplus/
)
