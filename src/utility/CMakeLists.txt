cmake_minimum_required(VERSION 3.5.1)

project(Utility C CXX)

include(../common.cmake)

file(GLOB_RECURSE sources *)
add_library(utility ${sources})

install(TARGETS utility DESTINATION lib)
install(FILES include/log.h DESTINATION include/service_kit)
