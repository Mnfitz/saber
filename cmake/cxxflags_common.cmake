

# This is unfortunately still needed to disable exceptions/RTTI since modern CMake still has no builtin support...
# E.g. replace_cxx_flag("/EHsc", "/EHs-c-")
macro(replace_cxx_flag pattern text)
    foreach (flag
        CMAKE_CXX_FLAGS CMAKE_CXX_FLAGS_DEBUG CMAKE_CXX_FLAGS_RELEASE
        CMAKE_CXX_FLAGS_MINSIZEREL CMAKE_CXX_FLAGS_RELWITHDEBINFO)

        string(REGEX REPLACE "${pattern}" "${text}" ${flag} "${${flag}}")

    endforeach()
endmacro()

# Fixup default compiler settings
if (MSVC)
	# Addn'l -D #defines to make #include<windows.h> less painful
	add_definitions(-DNOMINMAX=1 -DWIN32_LEAN_AND_MEAN=1)
	add_compile_options(
        # Use strict warning levels
        /W4 /WX)

	# For some unknown reason, 'RelWithDebInfo' compiles with '/Ob1' as opposed to '/Ob2' which prevents inlining of
	# functions not marked 'inline'. The reason we prefer 'RelWithDebInfo' over 'Release' is to get debug info, so manually
	# revert to the desired (and default) inlining behavior as that exercises more interesting code paths
	if ("${CMAKE_BUILD_TYPE}" STREQUAL "RelWithDebInfo")
		# TODO: This is currently blocked by an apparent Clang bug: https://github.com/llvm/llvm-project/issues/59690
		# replace_cxx_flag("/Ob1" "/Ob2")
		replace_cxx_flag("/Ob1" "/Ob2")
	endif() # STREQUAL "RelWithDebInfo"

	# Turn on "multi-procssor compilation" switch
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /MP")

else() # !MSVC
    # Clang with non-MSVC commandline syntax
    add_compile_options(
        # Effectively the same as /W4 /WX
        -Wall -Werror
    )
endif() # !MSVC

# Xcode: Enable some extra debug-facilities for debug builds...
if(XCODE)
	if("${CMAKE_BUILD_TYPE}" STREQUAL "Debug")
		# Generate XCode schema files
		set(CMAKE_XCODE_GENERATE_SCHEME TRUE)
		# Make malloc write 0xAA to newly allocated memory and 0x55 to deallocated memory
		set(CMAKE_XCODE_SCHEME_MALLOC_SCRIBBLE YES)
		# Place guard pages on each side of large (4096 bytes or more) buffers
		set(CMAKE_XCODE_SCHEME_MALLOC_GUARD_EDGES YES)
	endif() # STREQUAL "Debug"

endif() # XCODE

# Clang: Turn off misc warnings...
if (CMAKE_CXX_COMPILER_ID MATCHES "Clang")
    add_compile_options(
        # Ignore some pedantic warnings enabled by '-Wextra'
        -Wno-missing-field-initializers

        # Ignore some pedantic warnings enabled by '-Wpedantic'
        -Wno-language-extension-token
        -Wno-c++17-attribute-extensions
        -Wno-gnu-zero-variadic-macro-arguments
        -Wno-extra-semi

        # For tests, we want to be able to test self assignment, so disable this warning
        -Wno-self-assign-overloaded
        -Wno-self-move

        # clang needs this to enable _InterlockedCompareExchange128
        -mcx16

        # We don't want legacy MSVC conformance
        -fno-delayed-template-parsing

        # NOTE: Windows headers not clean enough for us to realistically attempt to start fixing these errors yet. That
        # said, errors that originate from WIL headers may benefit
        # -fno-ms-compatibility
        # -ferror-limit=999
        # -fmacro-backtrace-limit=0

        # -fno-ms-compatibility turns off preprocessor compatability, which currently only works when __VA_OPT__ support
        # is available (i.e. >= C++20)
        # -Xclang -std=c++2a
        )
else()
    add_compile_options(
        # We want to be as conformant as possible, so tell MSVC to not be permissive (note that this has no effect on clang-cl)
        /permissive-

        # wistd::function has padding due to alignment. This is expected
        /wd4324

        # TODO: https://github.com/Microsoft/wil/issues/6
        # /experimental:preprocessor

        # CRT headers are not yet /experimental:preprocessor clean, so work around the known issues
        # /Wv:18

        # Some tests have a LOT of template instantiations
        /bigobj

        # NOTE: Temporary workaround while https://github.com/microsoft/wil/issues/102 is being investigated
        /d2FH4-
        )
endif()
