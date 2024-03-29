cmake_minimum_required(VERSION 3.5.1)

project(HelloWorld C CXX)


include(../../src/common.cmake)

include_directories("${SERVICE_KIT_INSTALL_DIR}/include")
link_directories("${SERVICE_KIT_INSTALL_DIR}/lib")

# Targets greeter_[async_](client|server)
foreach(_target
    greeter_client greeter_server)
    add_executable(${_target} "${_target}.cc")

    target_link_libraries(${_target}
        server_sdk
        client_sdk
        rc_grpc_proto
        registry_center_proto
        ${_BOOST_LIBRARIES}
        ${_REFLECTION}
        ${_GRPC_GRPCPP}
        ${_PROTOBUF_LIBPROTOBUF})

    install(TARGETS ${_target}
            DESTINATION bin
            PERMISSIONS OWNER_WRITE OWNER_READ OWNER_EXECUTE GROUP_READ GROUP_EXECUTE WORLD_READ WORLD_EXECUTE)
endforeach()

if(NOT MSVC)
    install(FILES ${SERVICE_KIT_INSTALL_DIR}/bin/registry_center
            DESTINATION bin
            PERMISSIONS OWNER_WRITE OWNER_READ OWNER_EXECUTE GROUP_READ GROUP_EXECUTE WORLD_READ WORLD_EXECUTE)
else()
    string(REPLACE "\\" "/" SERVICE_KIT_INSTALL_DIR_FIXED ${SERVICE_KIT_INSTALL_DIR})
    install(FILES ${SERVICE_KIT_INSTALL_DIR_FIXED}/bin/registry_center.exe
            DESTINATION bin
            PERMISSIONS OWNER_WRITE OWNER_READ OWNER_EXECUTE GROUP_READ GROUP_EXECUTE WORLD_READ WORLD_EXECUTE)
endif()