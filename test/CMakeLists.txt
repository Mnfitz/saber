message(STATUS "Building saber/test/...")

include(FetchContent)
include(${PROJECT_SOURCE_DIR}/cmake/cxxflags_common.cmake)

if(SABER_BUILD_TESTS)

	FetchContent_Declare(
		Catch2
		GIT_REPOSITORY	https://github.com/catchorg/Catch2.git
		GIT_TAG			v3.6.0)

	FetchContent_MakeAvailable(Catch2)

	add_executable(sabre_test saber_test.cpp)
	# Catch2 provides a main function as executable entry point.
	# Private since there is no need to surface these libraries in the binary.
	target_link_libraries(sabre_test PRIVATE Catch2 Catch2WithMain)
	add_test(NAME sabre_test COMMAND test)

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
#	add_executable(sabre_benchmark benchmark.cpp benchmark_common.h)
#	add_test(NAME sabre_benchmark COMMAND test)
#	target_link_libraries(sabre_benchmark PUBLIC benchmark::benchmark)
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
#if (${FAST_BUILD})
#	if (MSVC)
#		replace_cxx_flag("/GR" "/GR-") # Disables RTTI
#	else()
#		add_compile_options(-fno-rtti)
#	endif()
#
#	add_definitions(-DCATCH_CONFIG_FAST_COMPILE -DWIL_FAST_BUILD)
#endif() # FAST_BUILD

# All projects need to reference the Saber headers
include_directories(${PROJECT_SOURCE_DIR}/include)

# Create a customized structure to hold header files when viewed through the IDE
# Header files will appear in an IDE folder called "Header Files"
source_group(
	TREE "include/${PROJECT_SOURCE_DIR}"
	PREFIX "Header Files"
	FILES ${HEADER_LIST})

include_directories(BEFORE SYSTEM ${CMAKE_BINARY_DIR}/include)

set(COMMON_SOURCES
	${CMAKE_CURRENT_SOURCE_DIR}/saber_test.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/../natvis/saber.natvis)

# find_package(Catch2 CONFIG REQUIRED)
link_libraries(Catch2::Catch2WithMain)

set(CMAKE_CXX_STANDARD 17)

if (MSVC)
	add_definitions(-DNOMINMAX=1 -DWIN32_LEAN_AND_MEAN=1)
	add_link_options(/NATVIS:${CMAKE_SOURCE_DIR}/natvis/saber.natvis)

	# For some unknown reason, 'RelWithDebInfo' compiles with '/Ob1' as opposed to '/Ob2' which prevents inlining of
	# functions not marked 'inline'. The reason we prefer 'RelWithDebInfo' over 'Release' is to get debug info, so manually
	# revert to the desired (and default) inlining behavior as that exercises more interesting code paths
	if ("${CMAKE_BUILD_TYPE}" STREQUAL "RelWithDebInfo")
		# TODO: This is currently blocked by an apparent Clang bug: https://github.com/llvm/llvm-project/issues/59690
		# replace_cxx_flag("/Ob1" "/Ob2")
		replace_cxx_flag("/Ob1" "/Ob2")
	endif() # STREQUAL "RelWithDebInfo"
endif() # MSVC

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