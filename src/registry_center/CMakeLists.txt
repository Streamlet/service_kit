cmake_minimum_required(VERSION 3.5.1)

project(RegistryCenter C CXX)

include(../common.cmake)

file(GLOB_RECURSE sources *)
add_executable(registry_center ${sources})
target_link_libraries(registry_center
    utility
    rc_grpc_proto
    ${_BOOST_LIBRARIES}
    ${_REFLECTION}
    ${_GRPC_GRPCPP}
    ${_PROTOBUF_LIBPROTOBUF})

install(TARGETS registry_center
        DESTINATION bin
        PERMISSIONS OWNER_WRITE OWNER_READ OWNER_EXECUTE GROUP_READ GROUP_EXECUTE WORLD_READ WORLD_EXECUTE)
if(NOT MSVC)
    install(FILES ${CMAKE_CURRENT_BINARY_DIR}/librc_grpc_proto.a
            DESTINATION lib
            RENAME libregistry_center_proto.a)
else()
    install(FILES ${CMAKE_CURRENT_BINARY_DIR}/${CMAKE_BUILD_TYPE}/rc_grpc_proto.lib
            DESTINATION lib
            RENAME registry_center_proto.lib)
endif()
