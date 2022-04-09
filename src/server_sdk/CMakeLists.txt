cmake_minimum_required(VERSION 3.5.1)

project(ServerSDK C CXX)

include(../common.cmake)

file(GLOB_RECURSE sources *)
add_library(server_sdk ${sources})

install(TARGETS server_sdk DESTINATION lib)
install(FILES include/server_sdk.h DESTINATION include/service_kit)
