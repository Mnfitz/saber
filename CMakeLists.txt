
cmake_minimum_required(VERSION 3.15)

project(
	saber
	VERSION		1.0
	DESCRIPTION	"Saber Utilities C++ Header Library")

# Organizes project into folders to reduce clutter
set_property(GLOBAL PROPERTY USE_FOLDERS ON)

include(GNUInstallDirs)

# From ./.vscode/settings.json: "cmake.configureSettings"
option(SABER_BUILD_TESTS "Build Unit Tests" OFF)
option(SABER_BUILD_BENCHMARKS "Build Benchmarks" OFF)
option(SABER_BUILD_DOCS "Build Doxygen Documentation" OFF)

# Add saber headers to #include searchpath
include_directories(include)

if(SABER_BUILD_TESTS)
	# Must be top-level or test won't be found.
	include(CTest) # enable_testing() invoked here!
	add_subdirectory(test)
endif() # SABER_BUILD_TESTS

# if(SABER_BUILD_DOCS)
#	add_subdirectory(doc/assets) # Build docset assets
#	add_subdirectory(doc/doxygen) # Build docset doxygen output
# endif() # SABER_BUILD_DOCS

# Gather headers into an interface library.
file(GLOB_RECURSE HEADER_FILES "${PROJECT_SOURCE_DIR}/include/${PROJECT_NAME}/*.h")
add_library(${PROJECT_NAME} INTERFACE)
add_library(${PROJECT_NAME}::${PROJECT_NAME} ALIAS ${PROJECT_NAME})

# The interface's include directory.
target_include_directories(${PROJECT_NAME} INTERFACE
	$<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
	$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>)

# Include the .natvis files
if (MSVC)
	target_sources(${PROJECT_NAME} INTERFACE
		"$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/natvis/${PROJECT_NAME}.natvis>")
endif() # MSVC

# Install Package Configuration
string(TOLOWER ${PROJECT_NAME} PROJECT_NAME_LOWER)
install(TARGETS ${PROJECT_NAME} EXPORT ${PROJECT_NAME_LOWER}_targets)
install(EXPORT ${PROJECT_NAME_LOWER}_targets
	NAMESPACE ${PROJECT_NAME}::
	FILE ${PROJECT_NAME_LOWER}Config.cmake
	DESTINATION "${CMAKE_INSTALL_DATADIR}/cmake/${PROJECT_NAME}")

# Install the headers at a standard cmake location.
install(DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/include/${PROJECT_NAME}" DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}")
