cmake_minimum_required(VERSION 3.5.1)

project(ClientSDK C CXX)

include(../common.cmake)

file(GLOB_RECURSE sources *)
add_library(client_sdk ${sources})

install(TARGETS client_sdk DESTINATION lib)
install(FILES include/client_sdk.h DESTINATION include/service_kit)
