cmake_minimum_required(VERSION 3.5.1)

project(ServiceKit C CXX)

add_subdirectory(utility)
add_subdirectory(registry_center)
target_include_directories(registry_center PUBLIC utility/include)

add_subdirectory(server_sdk)
add_subdirectory(client_sdk)
