cmake_minimum_required(VERSION 2.8.9)
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_BUILD_TYPE DEBUG)

project(bitmex_md_test)

include_directories(../../utils/libwebsockets/include/)
include_directories(../../utils/cpr/include/)
include_directories(../../utils/rapidjson/include/)
LINK_DIRECTORIES(../../utils/libwebsockets/lib)

find_package(crypto)
set(CORELIBS crypto)

set(SOURCE_FILES MDEngineBitmexTest.cpp)
add_executable(bitmex_md_test ${SOURCE_FILES})
TARGET_LINK_LIBRARIES(bitmex_md_test -lwebsockets)
