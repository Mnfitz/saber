cmake_minimum_required(VERSION 3.10)

project(
	saber
	VERSION		1.0
	DESCRIPTION	"Saber Utilities C++ Header Library")

message(STATUS "Building saber/...")

# Organizes project into folders to reduce clutter
set_property(GLOBAL PROPERTY USE_FOLDERS ON)

include(GNUInstallDirs)

# Set the target cpu architecture
string(TOLOWER ${CMAKE_SYSTEM_PROCESSOR} TARGET_ARCH)
if(TARGET_ARCH MATCHES "amd64" OR TARGET_ARCH MATCHES "x86_64")
	set(TARGET_ARCH "x64")
elseif(TARGET_ARCH MATCHES "arm64" OR TARGET_ARCH MATCHES "aarch64")
	set(TARGET_ARCH "arm64")
elseif(TARGET_ARCH MATCHES "x86" OR TARGET_ARCH MATCHES "i686")
	set(TARGET_ARCH "x86")
else()
	message(FATAL_ERROR "Unsupported target cpu architecture: ${TARGET_ARCH}")
endif()

# Add "./cmake" to the module path for include()'s
list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")

# Include external dependent projects
include(CatchOrg.Catch2)
# include(Microsoft.CppWinRT)
# include(Microsoft.Graphics.Win2D)
# include(Microsoft.Wil)
# include(Microsoft.WindowsAppSDK)

# Enable using ./.vscode/settings.json: "cmake.configureSettings"
option(SABER_BUILD_UNITTESTS "Build Unit Tests" OFF)
option(SABER_BUILD_BENCHMARKS "Build Benchmarks" OFF)
option(SABER_BUILD_DOCS "Build Doxygen Documentation" OFF)

# Add saber headers to the project's #include searchpath...
# (Note: "include", and not "src"... since saber is a header-only library)
include_directories(include) # (include) meaning: "./include/"

# Gather headers into an interface library...
file(GLOB_RECURSE HEADER_FILES
	"${PROJECT_SOURCE_DIR}/include/${PROJECT_NAME}/*.h"
	"${PROJECT_SOURCE_DIR}/include/${PROJECT_NAME}/*.hpp")

add_library(${PROJECT_NAME} INTERFACE)
add_library(${PROJECT_NAME}::${PROJECT_NAME} ALIAS ${PROJECT_NAME})

# The interface's include directory...
target_include_directories(${PROJECT_NAME} INTERFACE
	$<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
	$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>)

# Include compiler options necessary for building on this system
include(${PROJECT_SOURCE_DIR}/cmake/cxxflags_common.cmake)

# Avoid warning about DOWNLOAD_EXTRACT_TIMESTAMP in CMake 3.24:
if(CMAKE_VERSION VERSION_GREATER_EQUAL "3.24.0")
	# Weird: FetchContent(doxygen-awesome-css) needs this or it generates a big build warning
	cmake_policy(SET CMP0135 NEW)
endif() # if(CMAKE_VERSION)

# Saber is "at least c++17"
set(CMAKE_CXX_STANDARD 17)
# Don't require internet connectivity if FetchContent_XXX targets have already downloaded
set(FETCHCONTENT_UPDATES_DISCONNECTED ON)

if (WIN32)
	add_link_options(/NATVIS:${CMAKE_SOURCE_DIR}/natvis/${PROJECT_NAME}.natvis)
	# Include any .natvis (custom formatter for types viewed in the debugger) files
	target_sources(${PROJECT_NAME} INTERFACE
		"$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/natvis/${PROJECT_NAME}.natvis>")
endif() # WIN32

# Install Package Configuration
string(TOLOWER ${PROJECT_NAME} PROJECT_NAME_LOWER)
install(TARGETS ${PROJECT_NAME} EXPORT ${PROJECT_NAME_LOWER}_targets)
install(EXPORT ${PROJECT_NAME_LOWER}_targets
	NAMESPACE ${PROJECT_NAME}::
	FILE ${PROJECT_NAME_LOWER}Config.cmake
	DESTINATION "${CMAKE_INSTALL_DATADIR}/cmake/${PROJECT_NAME}")

# Install the headers at a standard cmake location.
install(DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/include/${PROJECT_NAME}" DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}")

if(SABER_BUILD_UNITTESTS)
	enable_testing()
	# Must be top-level or test won't be found.
	include(CTest) # enable_testing() invoked here!
	add_subdirectory(test)

endif() # SABER_BUILD_UNITTESTS

if(SABER_BUILD_DOCS)
	# add_subdirectory(doc/assets) # Build docset assets
	add_subdirectory(doc/doxygen) # Build docset doxygen output
endif() # SABER_BUILD_DOCS
