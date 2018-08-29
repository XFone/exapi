CMAKE_MINIMUM_REQUIRED(VERSION 2.8)

SET(LIBRARY_OUTPUT_PATH ${UTILS_LIB_BASE})

SET(build_target "iutils")

INCLUDE_DIRECTORIES (
	./osdeps
	${UTILS_H_BASE}
)

LINK_DIRECTORIES (
)

# generate project

FILE(GLOB iutils_h ${UTILS_H_BASE}/*.h)

FILE(GLOB src *.c)

#aux_source_directory(. src)

MESSAGE(STATUS ${src})

IF (WIN32)
#aux_source_directory(osdeps/win32 src_os)
FILE(GLOB src_os osdeps/win32/*.c*)
LIST(REMOVE_ITEM iutils_h ${UTILS_H_BASE}/RingBuff.h)
LIST(REMOVE_ITEM src ${PROJECT_SOURCE_DIR}/${build_target}/RingBuff.c)

MESSAGE(STATUS "win32:"${src})
ELSE (WIN32)

IF (UNIX)
FILE(GLOB src_os osdeps/linux/*.c*)
#aux_source_directory(osdeps/linux src_os)
ELSE (UNIX)
FILE(GLOB src_os osdeps/ucos2/*.c*)
#aux_source_directory(osdeps/ucos2 src_os)
ENDIF (UNIX)

endif (WIN32)

add_library(
	${build_target}
	STATIC
	
	${src}
	${src_os}
	${iutils_h}
)

ADD_DEFINITIONS(-D_LIB)
ADD_DEFINITIONS(-DNO_FILE_LINE)
IF (WIN32)

ADD_DEFINITIONS(-DDLL_EXPORT)

ELSE (WIN32)

ENDIF (WIN32)

set_source_files_properties(${build_target} PROPERTIES
       COMPILE_FLAGS  "${EXTRA_OPTIONS}")

get_target_property(target_file_name ${build_target} LOCATION)

add_custom_command(TARGET ${build_target} POST_BUILD 
                   COMMAND ${CMAKE_COMMAND} -E copy ${target_file_name} ${BIN_PATH}/$(Configuration)
				   )

set_target_properties( ${build_target} PROPERTIES LINKER_LANGUAGE "C")

#ADD_DEPENDENCIES(${build_target} Utilities)
