cmake_minimum_required(VERSION 3.5.1)

project(HelloWorld C CXX)

include(../../third_party/grpc/examples/cpp/cmake/common.cmake)

# Proto file
get_filename_component(rc_proto "../../protos/registry_center.proto" ABSOLUTE)
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

# Proto file
get_filename_component(hw_proto "../../third_party/grpc/examples/protos/helloworld.proto" ABSOLUTE)
get_filename_component(hw_proto_path "${hw_proto}" PATH)

# Generated sources
set(hw_proto_srcs "${CMAKE_CURRENT_BINARY_DIR}/helloworld.pb.cc")
set(hw_proto_hdrs "${CMAKE_CURRENT_BINARY_DIR}/helloworld.pb.h")
set(hw_grpc_srcs "${CMAKE_CURRENT_BINARY_DIR}/helloworld.grpc.pb.cc")
set(hw_grpc_hdrs "${CMAKE_CURRENT_BINARY_DIR}/helloworld.grpc.pb.h")
add_custom_command(
      OUTPUT "${hw_proto_srcs}" "${hw_proto_hdrs}" "${hw_grpc_srcs}" "${hw_grpc_hdrs}"
      COMMAND ${_PROTOBUF_PROTOC}
      ARGS --grpc_out "${CMAKE_CURRENT_BINARY_DIR}"
        --cpp_out "${CMAKE_CURRENT_BINARY_DIR}"
        -I "${hw_proto_path}"
        --plugin=protoc-gen-grpc="${_GRPC_CPP_PLUGIN_EXECUTABLE}"
        "${hw_proto}"
      DEPENDS "${hw_proto}")

# Include generated *.pb.h files
include_directories("${CMAKE_CURRENT_BINARY_DIR}")
include_directories("../../third_party/boost/built/include")
link_directories("../../third_party/boost/built/lib")

# hw_grpc_proto
add_library(hw_grpc_proto
  ${hw_grpc_srcs}
  ${hw_grpc_hdrs}
  ${hw_proto_srcs}
  ${hw_proto_hdrs})
target_link_libraries(hw_grpc_proto
  ${_REFLECTION}
  ${_GRPC_GRPCPP}
  ${_PROTOBUF_LIBPROTOBUF})

# Targets greeter_[async_](client|server)
foreach(_target
  greeter_client greeter_server)
  add_executable(${_target} "${_target}.cc")
  target_link_libraries(${_target}
    hw_grpc_proto
    rc_grpc_proto
    boost_program_options
    ${_REFLECTION}
    ${_GRPC_GRPCPP}
    ${_PROTOBUF_LIBPROTOBUF})
endforeach()
