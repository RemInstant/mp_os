cmake_minimum_required(VERSION 3.21)
project(mp_os_practice)


set(CMAKE_C_STANDARD 17)
set(CMAKE_CXX_STANDARD 17)

add_subdirectory(semaphores_client)
add_subdirectory(semaphores_server)
add_subdirectory(b_plus_tree)