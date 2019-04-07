cmake_minimum_required(VERSION 2.8.9)
set (CMAKE_CXX_STANDARD 11)
set(CMAKE_BUILD_TYPE DEBUG)

project (websocket_test)

include_directories(../../utils/libwebsockets/include/)
include_directories(../../utils/rapidjson/include/)
include_directories(/usr/local/ssl/include)
LINK_DIRECTORIES(../../utils/libwebsockets/lib)

find_package(crypto)
set(CORELIBS crypto)

set(SOURCE_FILES
	wstest.cpp
)


add_executable(websocket_test ${SOURCE_FILES})

TARGET_LINK_LIBRARIES(websocket_test -lwebsockets)
