cmake_minimum_required(VERSION 2.8)
project(ut)

set(SOURCES source/framework.cpp
            source/XMD5Test.cpp)

set(COMMON_LIBS XSDK)
set(WINDOWS_LIBS XSDK DbgHelp ws2_32 iphlpapi)

include(common.cmake NO_POLICY_SCOPE)
include("${devel_artifacts_path}/build/base_ut.cmake" NO_POLICY_SCOPE)
