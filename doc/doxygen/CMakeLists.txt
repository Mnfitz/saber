message(STATUS "Building saber/doc/doxygen/...")

include(FetchContent)

# saber_webdocs project depends on `doxygen-awesome` for the HTML
# nav-bar header used for navigating the doxygen generated doc set.
# See: https://github.com/jothepro/doxygen-awesome-css/tree/main?tab=readme-ov-file#cmake-with-fetchcontent
FetchContent_Declare(
	doxygen-awesome-css
	URL https://github.com/jothepro/doxygen-awesome-css/archive/refs/heads/main.zip)

#
# Saber "web docs" output directory defined here...
#
if(SABER_BUILD_DOCS)

	# Clone doxygen-awesome if needed...
	# FetchContent typically places the clone at: ./build/_deps/.
	FetchContent_MakeAvailable(doxygen-awesome-css)

	# Save the location the files were cloned into
	# This allows us to get the path to doxygen-awesome.css
	FetchContent_GetProperties(doxygen-awesome-css SOURCE_DIR DOXYGEN_AWESOME_DIR)

	find_package(Doxygen)
	if(NOT DOXYGEN_FOUND)
		message(SEND_ERROR	"Doxygen not found. The 'docs' target is not available. "
							"To generate documentation, you must install Doxygen, "
							"ensure that it's available in your $PATH, and re-run CMake.")
		return()
	endif() # NOT DOXYGEN_FOUND

	set(DOXYGEN_OUTPUT_DIR ${PROJECT_BINARY_DIR}/doc/webdocs)
	# Clean and rebuild output dir...
	file(REMOVE RECURSE ${DOXYGEN_OUTPUT_DIR})
	file(MAKE_DIRECTORY ${DOXYGEN_OUTPUT_DIR})

	# Doxyfile.in variable substitutions...

	# ${DOXYGEN_OUTPUT_DIR}
	set(DOXYGEN_INDEX_FILE ${DOXYGEN_OUTPUT_DIR}/index.html)
	# ${PROJECT_SOURCE_DIR}
	set(DOXYGEN_SRC_INPUT ${PROJECT_SOURCE_DIR}/include)
	set(DOXYGEN_MDFILE ${PROJECT_SOURCE_DIR}/README.md)
	# ${CMAKE_CURRENT_SOURCE_DIR}
	set(DOXYGEN_SRC_HEADER_FILE ${CMAKE_CURRENT_SOURCE_DIR}/html/header.html)
	set(DOXYGEN_SRC_FOOTER_FILE ${CMAKE_CURRENT_SOURCE_DIR}/html/footer.html)
	set(DOXYFILE_IN_FILE ${CMAKE_CURRENT_SOURCE_DIR}/Doxyfile.in)
	set(DOXYGEN_CSS_FILE ${CMAKE_CURRENT_SOURCE_DIR}/doxygen.css)
	# ${CMAKE_CURRENT_BINARY_DIR}
	set(DOXYFILE_OUT_FILE ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile)
	set(DOXYGEN_FOOTER_FILE ${CMAKE_CURRENT_BINARY_DIR}/footer.html)
	set(DOXYGEN_HEADER_FILE ${CMAKE_CURRENT_BINARY_DIR}/header.html)

	# Replace instances of @VARS@ from Doxyfile.in w/ current values from above...
	# Note: Doxyfile.in can reference doxygen-awesome-css via `HTML_EXTRA_STYLESHEET` field
	configure_file(${DOXYFILE_IN_FILE} ${DOXYFILE_OUT_FILE} @ONLY)

	# Yuck: Doofy client-side expansion for any `href-include` references
	# in header/footer since HTML only supports `href-include` on server-side.
	if(WIN32)
		find_program(GIT_BASH bash.exe HINTS "C:/Program Files/Git/bin")
		if(NOT GIT_BASH)
			message(SEND_ERROR "Git bash not found. Install Git for Windows and retry")
			return()
		endif() # NOT GIT_BASH_FOUND
		set(EXPAND_HREF_INCLUDES COMMAND ${GIT_BASH} -c "${CMAKE_CURRENT_SOURCE_DIR}/expand_href_includes.sh")
	else()
		set(EXPAND_HREF_INCLUDES COMMAND ${CMAKE_CURRENT_SOURCE_DIR}/expand_href_includes.sh)
	endif() # WIN32

	add_custom_command(
		COMMENT "Generating API documentation w/ Doxygen..."

		# COMMAND ${EXPAND_HREF_INCLUDES} ${DOXYGEN_SRC_HEADER_FILE} ${DOXYGEN_HEADER_FILE}
		# COMMAND ${EXPAND_HREF_INCLUDES} ${DOXYGEN_SRC_FOOTER_FILE} ${DOXYGEN_FOOTER_FILE}
		COMMAND ${CMAKE_COMMAND} -E copy_if_different ${DOXYGEN_SRC_HEADER_FILE} ${DOXYGEN_HEADER_FILE}
		COMMAND ${CMAKE_COMMAND} -E copy_if_different ${DOXYGEN_SRC_FOOTER_FILE} ${DOXYGEN_FOOTER_FILE}
		COMMAND ${CMAKE_COMMAND} -E copy_directory_if_different ${PROJECT_SOURCE_DIR}/doc ${DOXYGEN_OUTPUT_DIR}

		# Invoke Doxygen command and generate output ./build/doc/webdocs dir
		COMMAND ${DOXYGEN_EXECUTABLE} ${DOXYFILE_OUT_FILE}
		# Update timestamp of ${DOXYFILE_IN_FILE} to avoid rebuild w/o any changes
		COMMAND ${CMAKE_COMMAND} -E touch ${DOXYFILE_IN_FILE}
		
		WORKING_DIRECTORY ${DOXYGEN_OUTPUT_DIR}
		# Use HTML index file as documentation build dependency
		OUTPUT ${DOXYGEN_INDEX_FILE}
		# Run custom command based on state of Doxyfile.in
		DEPENDS ${DOXYFILE_IN_FILE} ${DOXYGEN_SRC_FOOTER_FILE} ${DOXYGEN_SRC_HEADER_FILE}
		VERBATIM)

	# HTML index file is used as documentation build dependency
	add_custom_target(saber_webdocs ALL DEPENDS ${DOXYGEN_INDEX_FILE})

	# Require a saber source code compile from which to generate webdocs
	add_dependencies(saber_webdocs saber)

endif() # SABER_BUILD_DOCS
