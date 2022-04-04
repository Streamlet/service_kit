cmake_minimum_required(VERSION 3.5.1)

set (CMAKE_CXX_STANDARD 17)

if(MSVC)
    add_definitions(-D_WIN32_WINNT=0x0600)
    add_definitions(-DBOOST_ALL_NO_LIB)
endif()

if(NOT DEFINED GRPC_INSTALL_DIR)
    message("GRPC_INSTALL_DIR must be defined")
endif()
if(NOT DEFINED THIRD_PARTY_INSTALL_DIR)
    message("THIRD_PARTY_INSTALL_DIR must be defined")
endif()
if(NOT DEFINED CMAKE_INSTALL_PREFIX)
    message("CMAKE_INSTALL_PREFIX must be defined")
endif()

include_directories("${CMAKE_CURRENT_SOURCE_DIR}")

include_directories("${GRPC_INSTALL_DIR}/include")
link_directories("${GRPC_INSTALL_DIR}/lib")

include_directories("${THIRD_PARTY_INSTALL_DIR}/include")
link_directories("${THIRD_PARTY_INSTALL_DIR}/lib")

if(NOT MSVC)
    set(Protobuf_DIR ${GRPC_INSTALL_DIR}/lib/cmake/protobuf)
else()
    set(Protobuf_DIR ${GRPC_INSTALL_DIR}/cmake)
endif()
set(gRPC_DIR ${GRPC_INSTALL_DIR}/lib/cmake/grpc)
set(absl_DIR ${GRPC_INSTALL_DIR}/lib/cmake/absl)

find_package(Threads)

find_package(Protobuf CONFIG REQUIRED)
message(STATUS "Using protobuf ${Protobuf_VERSION}")
set(_PROTOBUF_LIBPROTOBUF protobuf::libprotobuf)
set(_REFLECTION gRPC::grpc++_reflection)
set(_PROTOBUF_PROTOC $<TARGET_FILE:protobuf::protoc>)
find_package(gRPC CONFIG REQUIRED)
message(STATUS "Using gRPC ${gRPC_VERSION}")
set(_GRPC_GRPCPP gRPC::grpc++)
set(_GRPC_CPP_PLUGIN_EXECUTABLE $<TARGET_FILE:gRPC::grpc_cpp_plugin>)


file(GLOB_RECURSE rc_proto_list *.proto)
set(rc_proto_sources)
foreach(rc_proto ${rc_proto_list})
    get_filename_component(rc_proto_path "${rc_proto}" PATH)
    get_filename_component(rc_proto_name "${rc_proto}" NAME_WLE)
    set(rc_proto_h "${rc_proto_path}/${rc_proto_name}.pb.h")
    set(rc_proto_cc "${rc_proto_path}/${rc_proto_name}.pb.cc")
    set(rc_proto_grpc_h "${rc_proto_path}/${rc_proto_name}.grpc.pb.h")
    set(rc_proto_grpc_cc "${rc_proto_path}/${rc_proto_name}.grpc.pb.cc")
    add_custom_command(
        OUTPUT "${rc_proto_h}" "${rc_proto_cc}" "${rc_proto_grpc_h}" "${rc_proto_grpc_cc}"
        COMMAND ${_PROTOBUF_PROTOC}
        ARGS --grpc_out "${rc_proto_path}"
            --cpp_out "${rc_proto_path}"
            -I "${rc_proto_path}"
            --plugin=protoc-gen-grpc="${_GRPC_CPP_PLUGIN_EXECUTABLE}"
            "${rc_proto}"
        DEPENDS "${rc_proto}")
    list(APPEND rc_proto_sources ${rc_proto_h} ${rc_proto_cc} ${rc_proto_grpc_h} ${rc_proto_grpc_cc})
endforeach()

list(LENGTH rc_proto_sources rc_proto_sources_length)

if (${rc_proto_sources_length} GREATER 0)
    add_library(rc_grpc_proto ${rc_proto_sources})
    target_link_libraries(rc_grpc_proto
        ${_REFLECTION}
        ${_GRPC_GRPCPP}
        ${_PROTOBUF_LIBPROTOBUF})
endif()
