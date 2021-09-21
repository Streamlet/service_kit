cmake_minimum_required(VERSION 3.5.1)

project(RegistryCenter C CXX)

include(../common.cmake)

file(GLOB_RECURSE sources *)
add_executable(registry_center ${sources})
target_link_libraries(registry_center
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

install(TARGETS registry_center
        DESTINATION bin
        PERMISSIONS OWNER_WRITE OWNER_READ OWNER_EXECUTE GROUP_READ GROUP_EXECUTE WORLD_READ WORLD_EXECUTE)
install(FILES ${CMAKE_CURRENT_BINARY_DIR}/librc_grpc_proto.a
        DESTINATION lib
        RENAME libregistry_center_proto.a)
