
message("devel_artifacts path: ${devel_artifacts_path}")

# First, append our compile options...
#

set(CMAKE_BUILD_TYPE Debug)

macro(add_compiler_flag CONFIG FLAG)
    if("${CONFIG}" STREQUAL "Both")
        set(CMAKE_CXX_FLAGS "${FLAG} ${CMAKE_CXX_FLAGS}")
        set(CMAKE_CXX_FLAGS_RELEASE "${FLAG} ${CMAKE_CXX_FLAGS_RELEASE}")
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

    add_definitions(-DWIN32_LEAN_AND_MEAN)
    add_definitions(-DWIN32)
    add_definitions(-DUNICODE)
    add_definitions(-D_UNICODE)
    add_definitions(-DNOMINMAX)
    add_definitions(-D_CRT_SECURE_NO_WARNINGS)
    add_definitions(-D_CRT_NONSTDC_NO_DEPRECATE)
    add_definitions(-D__inline__=__inline)
    add_definitions(-D_SCL_SECURE_NO_WARNINGS)
    add_compiler_flag(Both /MT)
    # Enable read-only string pooling.
    add_compiler_flag(Both /GF)
    # Make sure that destructors get executed when exceptions exit C++ code.
    add_compiler_flag(Both /EHsc)
    # Generate pdb files which support Edit and Continue Debugging
    add_compiler_flag(Debug /Zi)
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
include_directories(include "${devel_artifacts_path}/include")
link_directories("${devel_artifacts_path}/lib")

if(CMAKE_SYSTEM MATCHES "Linux-")
  include_directories("/usr/local")
  link_directories("/usr/local/lib")
endif(CMAKE_SYSTEM MATCHES "Linux-")

# Add our executable target
#

add_executable(${PROJECT_NAME} ${SOURCES})

# Add platform appropriate libraries correct targets...
#

if(CMAKE_SYSTEM MATCHES "Windows")
    target_link_libraries(${PROJECT_NAME} ${WINDOWS_LIBS} ${COMMON_LIBS})
elseif(CMAKE_SYSTEM MATCHES "Linux")
    target_link_libraries(${PROJECT_NAME} ${LINUX_LIBS} ${COMMON_LIBS})
endif(CMAKE_SYSTEM MATCHES "Windows")
