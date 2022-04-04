cmake_minimum_required(VERSION 3.5.1)

project(ClientSDK C CXX)

include(../common.cmake)

file(GLOB_RECURSE sources *)
add_library(client_sdk ${sources})
target_link_libraries(client_sdk
    utility
    rc_grpc_proto
    boost_program_options
    boost_filesystem
    boost_thread
    boost_regex
    boost_log
    boost_log_setup
    ${_REFLECTION}
    ${_GRPC_GRPCPP}
    ${_PROTOBUF_LIBPROTOBUF})
if(NOT MSVC)
    target_link_libraries(client_sdk
        boost_program_options
        boost_filesystem
        boost_thread
        boost_regex
        boost_log
        boost_log_setup)
else()
    target_link_libraries(client_sdk
        libboost_program_options
        libboost_filesystem
        libboost_thread
        libboost_regex
        libboost_log
        libboost_log_setup)
endif()

install(TARGETS client_sdk DESTINATION lib)
install(FILES include/client_sdk.h DESTINATION include/service_kit)
