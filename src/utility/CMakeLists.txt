cmake_minimum_required(VERSION 3.5.1)

project(Utility C CXX)

include(../common.cmake)


file(GLOB_RECURSE sources *)
add_library(utility ${sources})
if(NOT MSVC)
    target_link_libraries(utility
        boost_filesystem
        boost_thread
        boost_regex
        boost_log
        boost_log_setup)
else()
    target_link_libraries(utility
        libboost_filesystem
        libboost_thread
        libboost_regex
        libboost_log
        libboost_log_setup)
endif()

install(TARGETS utility DESTINATION lib)
install(FILES include/log.h DESTINATION include/service_kit)
