# --- CatchOrg/Catch2 ---

include_guard(GLOBAL)

include(FetchContent)

# Target version to use for catchorg/catch2
set(catch2_VERSION "3.6.0")

# See: https://github.com/catchorg/Catch2/blob/v2.x/docs/cmake-integration.md#cmake-target
FetchContent_Declare(
	catch2
	GIT_REPOSITORY	https://github.com/catchorg/Catch2.git
	GIT_TAG			"v${catch2_VERSION}")

# FetchContent places this clone at: ./build/_deps/catch2-*.
FetchContent_MakeAvailable(catch2)

# Define an INTERFACE library for Catch2
add_library(CatchOrg.Catch2 INTERFACE)

set(catch2_INCLUDE_DIR ${catch2_SOURCE_DIR}/src)
set(catch2_LINK_LIBRARIES "Catch2::Catch2;Catch2::Catch2WithMain")

# Specify include directories
target_include_directories(CatchOrg.Catch2 INTERFACE
	${catch2_INCLUDE_DIR})

# Link against Catch2
target_link_libraries(CatchOrg.Catch2 INTERFACE
	${catch2_LINK_LIBRARIES})

# Create package configuration file required by cmake
#install(TARGETS CatchOrg.Catch2 EXPORT CatchOrg.Catch2Config)