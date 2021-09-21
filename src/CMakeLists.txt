cmake_minimum_required(VERSION 3.5.1)

project(ServiceKit C CXX)

add_subdirectory(utility)
add_subdirectory(registry_center)
target_include_directories(registry_center PUBLIC utility)

add_subdirectory(server_sdk)
target_include_directories(server_sdk PUBLIC utility)
target_include_directories(server_sdk PUBLIC registry_center)

add_subdirectory(client_sdk)
target_include_directories(client_sdk PUBLIC utility)
target_include_directories(client_sdk PUBLIC registry_center)
