
# First, append our compile options...
#

IF(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Debug)
ENDIF(NOT CMAKE_BUILD_TYPE)

macro(add_compiler_flag CONFIG FLAG)
    if("${CONFIG}" STREQUAL "Both")
        set(CMAKE_CXX_FLAGS "${FLAG} ${CMAKE_CXX_FLAGS}")
    elseif("${CONFIG}" STREQUAL "Debug")
        set(CMAKE_CXX_FLAGS_DEBUG "${FLAG} ${CMAKE_CXX_FLAGS_DEBUG}")
    elseif("${CONFIG}" STREQUAL "Release")
        set(CMAKE_CXX_FLAGS_RELEASE "${FLAG} ${CMAKE_CXX_FLAGS_RELEASE}")
    else()
        message(FATAL_ERROR "The CONFIG argument to add_compiler_flag must be \"Both\", \"Debug\", or \"Release\"")
    endif()
endmacro()

if(CMAKE_SYSTEM MATCHES "Linux-")
    add_compiler_flag(Both -fthreadsafe-statics)
    add_compiler_flag(Both -fPIC)
    add_compiler_flag(Release -O3)
    add_compiler_flag(Release -DNDEBUG)
    add_compiler_flag(Debug -ggdb)
    add_compiler_flag(Debug -O0)
    add_compiler_flag(Both -std=c++11)
    add_definitions(-D_LINUX)
    add_definitions(-DLINUX_OS)
    add_definitions(-D_REENTRANT)
    set(CMAKE_EXE_LINKER_FLAGS -rdynamic)
elseif(CMAKE_SYSTEM MATCHES "Windows")

    if(${CMAKE_SIZEOF_VOID_P} EQUAL 8)
        add_definitions(-DWIN64)
    endif(${CMAKE_SIZEOF_VOID_P} EQUAL 8)

    add_definitions(-DWIN32)
    add_definitions(-DUNICODE)
    add_definitions(-D_UNICODE)
    add_definitions(-DNOMINMAX)
    add_definitions(-D_CRT_SECURE_NO_WARNINGS)
    add_definitions(-D_CRT_NONSTDC_NO_DEPRECATE)
    add_definitions(-D__inline__=__inline)
    add_definitions(-D_SCL_SECURE_NO_WARNINGS)
    # Enable read-only string pooling.
    add_compiler_flag(Both /GF)
    # Make sure that destructors get executed when exceptions exit C++ code.
    add_compiler_flag(Both /EHsc)
    # Generate pdb files which support Edit and Continue Debugging
    add_compiler_flag(Debug /ZI)
    # Enable minimal rebuild
    add_compiler_flag(Debug /Gm)
    # Create an .sbr file with complete symbolic information.
    add_compiler_flag(Debug /FR)
    add_compiler_flag(Debug /MDd)
    add_compiler_flag(Release /MD)
    add_compiler_flag(Release /O2)
    SET_PROPERTY(GLOBAL PROPERTY USE_FOLDERS ON)
endif(CMAKE_SYSTEM MATCHES "Linux-")


# Now, setup our artifact install root and add our default header and lib paths.
#

set(CMAKE_INSTALL_PREFIX ${devel_artifacts_path})
get_filename_component(ABSOLUTE_INC_DIR "${devel_artifacts_path}/include" ABSOLUTE)
include_directories(include ${ABSOLUTE_INC_DIR})
get_filename_component(ABSOLUTE_LIB_DIR "${devel_artifacts_path}/lib" ABSOLUTE)
link_directories(${ABSOLUTE_LIB_DIR})

# rpath setup
#

SET(CMAKE_SKIP_BUILD_RPATH TRUE)
SET(CMAKE_BUILD_WITH_INSTALL_RPATH TRUE)

# if we have been provided with additional rpath dirs, use them.
IF(ADDITIONAL_RELATIVE_RPATH)
  SET(CMAKE_INSTALL_RPATH "\$ORIGIN/libs:\$ORIGIN/${ADDITIONAL_RELATIVE_RPATH}")
ELSE()
  SET(CMAKE_INSTALL_RPATH "\$ORIGIN/libs")
ENDIF()

SET(CMAKE_INSTALL_RPATH_USE_LINK_PATH TRUE)

# Add our executable target
#

# So basically, if we're building for Windows we include the "WIN32" in the
# add_executable to get it to build a real Windows application. All of the
# stuff in the "Windows" conditional is to get it to compile in our icon.

if(${APPLICATION_TYPE} STREQUAL "WINDOWS")
    set(RES_FILES ${RC_FILE})
    set(CMAKE_RC_COMPILER_INIT windres)
    ENABLE_LANGUAGE(RC)
    SET(CMAKE_RC_COMPILE_OBJECT
    "<CMAKE_RC_COMPILER> <FLAGS> -o coff <DEFINES> -i <SOURCE> -o <OBJECT>")
    add_executable(${PROJECT_NAME} WIN32 ${SOURCES} ${RES_FILES})
endif(${APPLICATION_TYPE} STREQUAL "WINDOWS")

if(${APPLICATION_TYPE} STREQUAL "WINDOWS_CONSOLE")
    set(RES_FILES ${RC_FILE})
    set(CMAKE_RC_COMPILER_INIT windres)
    ENABLE_LANGUAGE(RC)
    SET(CMAKE_RC_COMPILE_OBJECT
    "<CMAKE_RC_COMPILER> <FLAGS> -o coff <DEFINES> -i <SOURCE> -o <OBJECT>")
    add_executable(${PROJECT_NAME} ${SOURCES} ${RES_FILES})
endif(${APPLICATION_TYPE} STREQUAL "WINDOWS_CONSOLE")

if(${APPLICATION_TYPE} STREQUAL "NORMAL")
    add_executable(${PROJECT_NAME} ${SOURCES})
endif(${APPLICATION_TYPE} STREQUAL "NORMAL")

# Add our libraries...
#

if(CMAKE_SYSTEM MATCHES "Windows")
    target_link_libraries(${PROJECT_NAME} ${WINDOWS_LIBS} ${COMMON_LIBS})
elseif(CMAKE_SYSTEM MATCHES "Linux")
    target_link_libraries(${PROJECT_NAME} ${LINUX_LIBS} ${COMMON_LIBS})
endif(CMAKE_SYSTEM MATCHES "Windows")


