cmake_minimum_required(VERSION 3.6)

message(STATUS "Building saber/test/...")

#include(FetchContent)

# Both saber_unittest and saber_benchmark depend on `Catch2` test framework
# See: https://github.com/catchorg/Catch2/blob/v2.x/docs/cmake-integration.md#cmake-target
# FetchContent_Declare(
# 	Catch2
# 	GIT_REPOSITORY	https://github.com/catchorg/Catch2.git
# 	GIT_TAG			v3.6.0)

#
# Saber "unit test" app (saber_unittest) defined here
#
if(SABER_BUILD_UNITTESTS)

	# Clone Catch2 (if needed)...
	# FetchContent typically places this clone at: ./build/_deps/.
	# FetchContent_MakeAvailable(Catch2)

	# saber_unittest source files...
	set(SOURCE_FILES_UNITTEST
		${CMAKE_CURRENT_SOURCE_DIR}/geometry_unittest.cpp
		${CMAKE_CURRENT_SOURCE_DIR}/handler_unittest.cpp
		${CMAKE_CURRENT_SOURCE_DIR}/saber_unittest.cpp
		${CMAKE_CURRENT_SOURCE_DIR}/utility_unittest.cpp
		${CMAKE_CURRENT_SOURCE_DIR}/CMakeLists.txt)

	add_executable(saber_unittest
		${HEADER_FILES}
		${SOURCE_FILES_UNITTEST}
		${PROJECT_SOURCE_DIR}/CMakeLists.txt)

	# Add include paths for: "saber" and "catch2"
	target_include_directories(saber_unittest PRIVATE
		${CMAKE_BINARY_DIR}/include
		#${catch2_INCLUDE_DIR}
		${wil_INCLUDE_DIR}
		CatchOrg.Catch2)
		# Microsoft.Graphics.Win2D
		# Microsoft.WindowsAppSDK)

	# find_package(Catch2 CONFIG REQUIRED)
	# link_libraries(Catch2::Catch2WithMain)
	#find_package(CatchOrg.Catch2 REQUIRED)
	#find_package(Microsoft.WindowsAppSDK REQUIRED)

	# Catch2 provides a main function as executable entry point.
	# Private since there is no need to export these libraries in the binary.
	target_link_libraries(saber_unittest PRIVATE
		CatchOrg.Catch2)
		# Microsoft.Graphics.Win2D
		# Microsoft.WindowsAppSDK)

	add_test(NAME saber_unittest COMMAND test)

	# Make the Visual Studio .vcxproj files look prettier...
	if(WIN32)
		# TRICKY j3fitz 12may2024: Making `source_groups` appear in the VS IDE
		# A `source_group` directive will only take effect if the FILES
		# specified are also dependencies of the `add_executable`

		# Source files will appear in an IDE folder called "test"
		source_group(
			TREE "${PROJECT_SOURCE_DIR}/test"
			PREFIX "test"
			FILES ${SOURCE_FILES_UNITTEST})	

		# Header files will appear in an IDE folder called "include"
		source_group(
			TREE "${PROJECT_SOURCE_DIR}/include/${PROJECT_NAME}"
			PREFIX "include/${PROJECT_NAME}"
			FILES ${HEADER_FILES})

		# Turn off warnings caused by ReferenceHandler<std::FILE>/std::fopen() usage
		add_definitions(-D_CRT_SECURE_NO_WARNINGS=1)

		# Set saber_unittest target as the `startup project` for the generated .sln file
		set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT saber_unittest)

		endif() # WIN32
endif() # SABER_BUILD_UNITTESTS

if(SABER_BUILD_BENCHMARKS)
#
#	FetchContent_Declare(
#		googlebenchmark
#		GIT_REPOSITORY	https://github.com/google/benchmark.git
#		GIT_TAG			v1.8.3)
#
#	FetchContent_Declare(
#		googletest
#		GIT_REPOSITORY	https://github.com/google/googletest.git
#		GIT_TAG			v1.14.0)  
#
#	FetchContent_MakeAvailable(googlebenchmark googletest)
#

#	FetchContent_MakeAvailable(Catch2)

	# saber_benchmark source files...
	set(SOURCE_FILES_BENCHMARK
		${CMAKE_CURRENT_SOURCE_DIR}/geometry_benchmark.cpp
		${CMAKE_CURRENT_SOURCE_DIR}/handler_benchmark.cpp
		${CMAKE_CURRENT_SOURCE_DIR}/saber_benchmark.cpp
		${CMAKE_CURRENT_SOURCE_DIR}/CMakeLists.txt)

	add_executable(saber_benchmark
		${HEADER_FILES}
		${SOURCE_FILES_BENCHMARK}
		${PROJECT_SOURCE_DIR}/CMakeLists.txt)

	# Add include paths for: "saber" and "catch2"
	target_include_directories(saber_unittest PRIVATE
		${CMAKE_BINARY_DIR}/include
		${catch2_INCLUDE_DIR})

	# find_package(Catch2 CONFIG REQUIRED)
	# link_libraries(Catch2::Catch2WithMain)
	#find_package(CatchOrg.Catch2 REQUIRED)
	#find_package(Microsoft.WindowsAppSDK REQUIRED)

	# Catch2 provides a main function as executable entry point.
	# Private since there is no need to surface these libraries in the binary.
	target_link_libraries(saber_benchmark PRIVATE
		CatchOrg.Catch2)
		# Microsoft.Graphics.Win2D
		# Microsoft.WindowsAppSDK)

	add_test(NAME saber_benchmark COMMAND test)

	# Make the Visual Studio .vcxproj files look prettier...
	if(WIN32)
		# TRICKY j3fitz 12may2024: Making `source_groups` appear in the VS IDE
		# A `source_group` directive will only take effect if the FILES
		# specified are also dependencies of the `add_executable`

		# Source files will appear in an IDE folder called "test"
		source_group(
			TREE "${PROJECT_SOURCE_DIR}/test"
			PREFIX "test"
			FILES ${SOURCE_FILES_BENCHMARK})	

		# Header files will appear in an IDE folder called "include"
		source_group(
			TREE "${PROJECT_SOURCE_DIR}/include/${PROJECT_NAME}"
			PREFIX "include/${PROJECT_NAME}"
			FILES ${HEADER_FILES})

	endif() # WIN32
#
#	add_executable(saber_baseline benchmark_baseline.cpp benchmark_common.h)
#	add_test(NAME saber_baseline COMMAND test)
#	target_link_libraries(saber_baseline PUBLIC benchmark::benchmark)
#
#	py3_build(benchmark_report.py)
#
#	file(COPY benchmark.sh
#		DESTINATION ${CMAKE_CURRENT_BINARY_DIR} 
#		USE_SOURCE_PERMISSIONS)
#
endif() # SABER_BUILD_BENCHMARKS

# The build pipelines have limitations that local development environments do not, so turn a few knobs
#if(${FAST_BUILD})
#	if (WIN32)
#		replace_cxx_flag("/GR" "/GR-") # Disables RTTI
#	else()
#		add_compile_options(-fno-rtti)
#	endif()
#
#	add_definitions(-DCATCH_CONFIG_FAST_COMPILE -DWIL_FAST_BUILD)
#endif() # FAST_BUILD

#add_subdirectory(win32)
#add_subdirectory(osx)
#add_subdirectory(posix)

#set(DEBUG_BUILD FALSE)
#set(HAS_DEBUG_INFO FALSE)
#
#if ("${CMAKE_BUILD_TYPE}" STREQUAL "Debug")
#	set(DEBUG_BUILD TRUE)
#	set(HAS_DEBUG_INFO TRUE)
#elseif("${CMAKE_BUILD_TYPE}" STREQUAL "RelWithDebInfo")
#	set(HAS_DEBUG_INFO TRUE)
#endif()
# Release & MinSizeRel => keep all false

#set(ASAN_AVAILABLE FALSE)
#set(UBSAN_AVAILABLE FALSE)
#if (CMAKE_CXX_COMPILER_ID MATCHES "MSVC")
#	# Address Sanitizer is available for all architectures and build types, but warns/errors if debug info is not enabled
#	set(ASAN_AVAILABLE ${HAS_DEBUG_INFO})
#elseif (CMAKE_CXX_COMPILER_ID MATCHES "Clang")
#	# Sanitizers are not available with debug libraries
#	# Starting with Clang 17, there appears to be some issue where linking to the dynamic asan libraries seems to think that
#	# things like 'operator new' are present in the executable, but only for x64 for some reason
#	set(ASAN_AVAILABLE NOT ${DEBUG_BUILD} AND $ENV{Platform} STREQUAL "x86")
#	set(UBSAN_AVAILABLE NOT ${DEBUG_BUILD})
#endif()
#
#if (${ASAN_AVAILABLE})
#	add_subdirectory(sanitize-address)
#endif()
#
#if (${UBSAN_AVAILABLE})
#	# TODO: Disabled until https://github.com/microsoft/STL/issues/3568 is resolved
#	# add_subdirectory(sanitize-undefined-behavior)
#endif()