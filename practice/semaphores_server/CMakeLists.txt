cmake_minimum_required(VERSION 3.21)
project(mp_os_practice_smphr_srvr)

add_executable(
        mp_os_practice_smphr_srvr
        semaphores_server.c)
target_include_directories(
        mp_os_practice_smphr_srvr
        PUBLIC
        ./include)
set_target_properties(
        mp_os_practice_smphr_srvr PROPERTIES
        LANGUAGES C
        C_STANDARD 17
        C_STANDARD_REQUIRED ON
        C_EXTENSIONS OFF
        VERSION 1.0
        DESCRIPTION "practice")