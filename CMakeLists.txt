cmake_minimum_required(VERSION 3.26)

SET(PROJECT_AUTHOR "FlayaN")
project(
	commonlibsse-ng-submodule-template
	VERSION 0.0.1
	LANGUAGES CXX
)

# ########################################################################################################################
# ## Source build guard
# ########################################################################################################################

if(PROJECT_SOURCE_DIR STREQUAL PROJECT_BINARY_DIR)
	message(FATAL_ERROR "In-source builds not allowed. Please make a new directory (called a build directory) and run CMake from there.")
endif()

# ########################################################################################################################
# ## Build options
# ########################################################################################################################

macro(set_from_environment VARIABLE)
	if (NOT DEFINED ${VARIABLE} AND DEFINED ENV{${VARIABLE}})
		set(${VARIABLE} $ENV{${VARIABLE}})
	endif ()
endmacro()

set_from_environment(OUTPUT_DIRS)

message("Options:")
option(COPY_BUILD "Copy the build output to the Skyrim directory, also need OUTPUT_DIRS" OFF)
option(ZIP_TO_DIST "Zip the mod to 7z file in dist." ON)
message("\tCopy build output: ${COPY_BUILD}")
message("\t\tCopy build output path OUTPUT_DIRS: ${OUTPUT_DIRS}")
message("\tZip to dist: ${ZIP_TO_DIST}")

# #######################################################################################################################
# # Add CMake features
# #######################################################################################################################

configure_file(
	${CMAKE_CURRENT_SOURCE_DIR}/cmake/Plugin.h.in
	${CMAKE_CURRENT_BINARY_DIR}/cmake/Plugin.h
	@ONLY
)

configure_file(
	${CMAKE_CURRENT_SOURCE_DIR}/cmake/Version.rc.in
	${CMAKE_CURRENT_BINARY_DIR}/cmake/version.rc
	@ONLY
)

set_property(GLOBAL PROPERTY USE_FOLDERS ON)
set(CMAKE_INTERPROCEDURAL_OPTIMIZATION ON)
set(CMAKE_INTERPROCEDURAL_OPTIMIZATION_DEBUG OFF)
set(CommonLibPath "extern/CommonLibSSE-NG")

# #######################################################################################################################
# # Find dependencies
# #######################################################################################################################

find_path(CLIB_UTIL_INCLUDE_DIRS "ClibUtil/detail/SimpleIni.h")

# #######################################################################################################################
# # CommonLibSSE-NG Plugin
# #######################################################################################################################

include(${CommonLibPath}/cmake/CommonLibSSE.cmake)
add_subdirectory(${CommonLibPath})
add_commonlibsse_plugin(
	${PROJECT_NAME} 
	AUTHOR ${PROJECT_AUTHOR}
	SOURCES ${INCLUDE_HEADERS} ${SOURCE_HEADERS} ${SOURCES}
)

# #######################################################################################################################
# # Headers and sources
# #######################################################################################################################

include(${CMAKE_CURRENT_SOURCE_DIR}/cmake/AddCXXFiles.cmake)
add_cxx_files("${PROJECT_NAME}")

# #######################################################################################################################
# # Post target setup
# #######################################################################################################################

target_precompile_headers(
	${PROJECT_NAME}
	PRIVATE
		include/PCH.h
)

target_sources(
	${PROJECT_NAME}
	PRIVATE
		${CMAKE_CURRENT_BINARY_DIR}/cmake/Plugin.h
		${CMAKE_CURRENT_BINARY_DIR}/cmake/version.rc
)

target_compile_features(
	${PROJECT_NAME}
	PRIVATE
		cxx_std_23
)

target_include_directories(
	${PROJECT_NAME}
	PRIVATE
		${CMAKE_CURRENT_BINARY_DIR}/cmake
		${CLIB_UTIL_INCLUDE_DIRS}
)

target_link_libraries(
	${PROJECT_NAME}
	PRIVATE
		CommonLibSSE::CommonLibSSE
)

# https://gitlab.kitware.com/cmake/cmake/-/issues/24922
set_target_properties(
	${PROJECT_NAME}
	PROPERTIES
		VS_USER_PROPS ${CMAKE_CURRENT_SOURCE_DIR}/cmake/build_stl_modules.props
)

# #######################################################################################################################
# # Automatic deployment
# #######################################################################################################################

if (COPY_BUILD)
	if (DEFINED OUTPUT_DIRS)
		foreach(OUTPUT_DIR OUTPUT_DIRS)
			message("Copying mod to ${OUTPUT_DIR}/SKSE/Plugins")
			add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
				COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:${PROJECT_NAME}> ${OUTPUT_DIR}/SKSE/Plugins/
				COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_PDB_FILE:${PROJECT_NAME}> ${OUTPUT_DIR}/SKSE/Plugins/
			)
		endforeach()
	else ()
		message(
			WARNING
			"Variable OUTPUT_DIRS is not defined. Skipping post-build copy command."
		)
	endif ()
endif ()

if(ZIP_TO_DIST)
	set(ZIP_DIR "${CMAKE_CURRENT_BINARY_DIR}/zip")
	add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
		COMMAND ${CMAKE_COMMAND} -E remove_directory "${ZIP_DIR}" ${CMAKE_SOURCE_DIR}/dist
		COMMAND ${CMAKE_COMMAND} -E make_directory "${ZIP_DIR}/SKSE/Plugins" ${CMAKE_SOURCE_DIR}/dist
	)

	message("Copying mod to ${ZIP_DIR}.")
	add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
		COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:${PROJECT_NAME}> "${ZIP_DIR}/SKSE/Plugins/"
		COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_PDB_FILE:${PROJECT_NAME}> "${ZIP_DIR}/SKSE/Plugins/"
	)

	set(TARGET_ZIP "${PROJECT_NAME}${PROJECT_VERSION}.7z")
	message("Zipping ${ZIP_DIR} to ${CMAKE_SOURCE_DIR}/dist/${TARGET_ZIP}")
	ADD_CUSTOM_COMMAND(TARGET ${PROJECT_NAME} POST_BUILD
		COMMAND ${CMAKE_COMMAND} -E tar cf ${CMAKE_SOURCE_DIR}/dist/${TARGET_ZIP} --format=7zip -- .
		WORKING_DIRECTORY ${ZIP_DIR}
	)
endif()
