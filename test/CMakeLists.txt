cmake_minimum_required(VERSION 3.6)

message(STATUS "Building saber/test/...")

include(FetchContent)

#
# Saber "unit test" app (saber_test) defined here
#
if(SABER_BUILD_TESTS)

	# saber_test depends on `Catch2` test framework
	FetchContent_Declare(
		Catch2
		GIT_REPOSITORY	https://github.com/catchorg/Catch2.git
		GIT_TAG			v3.6.0)
	FetchContent_MakeAvailable(Catch2)

	# saber_test source files...
	set(SOURCE_FILES
#		${CMAKE_CURRENT_SOURCE_DIR}/handler.cpp
		${CMAKE_CURRENT_SOURCE_DIR}/saber_test.cpp
		${CMAKE_CURRENT_SOURCE_DIR}/CMakeLists.txt)

	add_executable(saber_test
		${HEADER_FILES}
		${SOURCE_FILES}
		${PROJECT_SOURCE_DIR}/CMakeLists.txt)

	include_directories(BEFORE SYSTEM ${CMAKE_BINARY_DIR}/include)

	# find_package(Catch2 CONFIG REQUIRED)
	link_libraries(Catch2::Catch2WithMain)

	# Catch2 provides a main function as executable entry point.
	# Private since there is no need to surface these libraries in the binary.
	target_link_libraries(saber_test PRIVATE Catch2 Catch2WithMain)
	add_test(NAME saber_test COMMAND test)

	# Make the Visual Studio .vcxproj files look prettier...
	if(MSVC)
		# TRICKY jfitz 12may2024: Making `source_groups` appear in the VS IDE
		# A `source_group` directive will only take effect if the FILES
		# specified are also dependencies of the `add_executable`

		# Source files will appear in an IDE folder called "test"
		source_group(
			TREE "${PROJECT_SOURCE_DIR}/test"
			PREFIX "test"
			FILES ${SOURCE_FILES})	

		# Header files will appear in an IDE folder called "include"
		source_group(
			TREE "${PROJECT_SOURCE_DIR}/include/${PROJECT_NAME}"
			PREFIX "include/${PROJECT_NAME}"
			FILES ${HEADER_FILES})

	endif() # MSVC
endif() # SABER_BUILD_TESTS

#if(SABER_BUILD_BENCHMARKS)
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
#	add_executable(saber_benchmark benchmark.cpp benchmark_common.h)
#	add_test(NAME saber_benchmark COMMAND test)
#	target_link_libraries(saber_benchmark PUBLIC benchmark::benchmark)
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
#endif() # SABER_BUILD_BENCHMARKS

# The build pipelines have limitations that local development environments do not, so turn a few knobs
#if(${FAST_BUILD})
#	if (MSVC)
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