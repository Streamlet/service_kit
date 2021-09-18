cmake_minimum_required(VERSION 3.5.1)

project(RegistryCenter C CXX)

include(../third_party/grpc/examples/cpp/cmake/common.cmake)

# Proto file
get_filename_component(rc_proto "../protos/registry_center.proto" ABSOLUTE)
get_filename_component(rc_proto_path "${rc_proto}" PATH)

# Generated sources
set(rc_proto_srcs "${CMAKE_CURRENT_BINARY_DIR}/registry_center.pb.cc")
set(rc_proto_hdrs "${CMAKE_CURRENT_BINARY_DIR}/registry_center.pb.h")
set(rc_grpc_srcs "${CMAKE_CURRENT_BINARY_DIR}/registry_center.grpc.pb.cc")
set(rc_grpc_hdrs "${CMAKE_CURRENT_BINARY_DIR}/registry_center.grpc.pb.h")
add_custom_command(
      OUTPUT "${rc_proto_srcs}" "${rc_proto_hdrs}" "${rc_grpc_srcs}" "${rc_grpc_hdrs}"
      COMMAND ${_PROTOBUF_PROTOC}
      ARGS --grpc_out "${CMAKE_CURRENT_BINARY_DIR}"
        --cpp_out "${CMAKE_CURRENT_BINARY_DIR}"
        -I "${rc_proto_path}"
        --plugin=protoc-gen-grpc="${_GRPC_CPP_PLUGIN_EXECUTABLE}"
        "${rc_proto}"
      DEPENDS "${rc_proto}")

# rc_grpc_proto
add_library(rc_grpc_proto
  ${rc_grpc_srcs}
  ${rc_grpc_hdrs}
  ${rc_proto_srcs}
  ${rc_proto_hdrs})
target_link_libraries(rc_grpc_proto
  ${_REFLECTION}
  ${_GRPC_GRPCPP}
  ${_PROTOBUF_LIBPROTOBUF})

# Include generated *.pb.h files
include_directories("${CMAKE_CURRENT_BINARY_DIR}")
include_directories("../third_party/boost/built/include")
link_directories("../third_party/boost/built/lib")

add_executable(registry_center "registry_center.cc")
target_link_libraries(registry_center
  rc_grpc_proto
  boost_program_options
  ${_REFLECTION}
  ${_GRPC_GRPCPP}
  ${_PROTOBUF_LIBPROTOBUF})
