cmake_minimum_required(VERSION 2.8.9)
set (CMAKE_CXX_STANDARD 11)

project (liandao_test)

include_directories(/usr/include/)
include_directories(../utils/rapidjson/include/)
add_executable(rapidjson_test rapidjson_test/rapidjson_test.cpp)

#find_package(crypto)
#set(CORELIBS crypto)

add_executable(openssl_util_test crypto_test/openssl_util_test.cpp)
target_link_libraries(openssl_util_test ${CORELIBS})

add_executable(binance_rapidjson_test binance_rapidjson_test/binance_rapidjson_test.cpp)
target_link_libraries(binance_rapidjson_test ${CORELIBS})

include_directories(../utils/cpr/include)
add_executable(binance_test binance_test/binance_test.cpp)
target_link_libraries(binance_test ${CORELIBS})
target_link_libraries(binance_test ${CMAKE_SOURCE_DIR}/utils/cpr/lib/libcpr.so ${CMAKE_SOURCE_DIR}/utils/cpr/lib/libcurl.so ${CMAKE_SOURCE_DIR}/utils/cpr/lib/libcrypto.so ${CMAKE_SOURCE_DIR}/utils/cpr/lib/libssl.so)
