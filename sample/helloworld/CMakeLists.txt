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
        rc_grpc_proto
        registry_center_proto
        server_sdk
        client_sdk
        ${_REFLECTION}
        ${_GRPC_GRPCPP}
        ${_PROTOBUF_LIBPROTOBUF})
    if(NOT MSVC)
        target_link_libraries(${_target}
            boost_program_options
            boost_filesystem
            boost_thread
            boost_regex
            boost_log
            boost_log_setup)
    else()
        target_link_libraries(${_target}
            libboost_program_options
            libboost_filesystem
            libboost_thread
            libboost_regex
            libboost_log
            libboost_log_setup)
    endif()
    install(TARGETS ${_target}
            DESTINATION bin
            PERMISSIONS OWNER_WRITE OWNER_READ OWNER_EXECUTE GROUP_READ GROUP_EXECUTE WORLD_READ WORLD_EXECUTE)
endforeach()

if(NOT MSVC)
    install(FILES ${SERVICE_KIT_INSTALL_DIR}/bin/registry_center
            DESTINATION bin
            PERMISSIONS OWNER_WRITE OWNER_READ OWNER_EXECUTE GROUP_READ GROUP_EXECUTE WORLD_READ WORLD_EXECUTE)
else()
    install(FILES ${SERVICE_KIT_INSTALL_DIR}/bin/registry_center.exe
            DESTINATION bin
            PERMISSIONS OWNER_WRITE OWNER_READ OWNER_EXECUTE GROUP_READ GROUP_EXECUTE WORLD_READ WORLD_EXECUTE)
endif()