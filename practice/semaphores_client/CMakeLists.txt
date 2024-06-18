cmake_minimum_required(VERSION 3.21)
project(mp_os_practice_smphr_clnt)

add_executable(
        mp_os_practice_smphr_clnt
        semaphores_client.c)
target_include_directories(
        mp_os_practice_smphr_clnt
        PUBLIC
        ./include)
set_target_properties(
        mp_os_practice_smphr_clnt PROPERTIES
        LANGUAGES C
        C_STANDARD 17
        C_STANDARD_REQUIRED ON
        C_EXTENSIONS OFF
        VERSION 1.0
        DESCRIPTION "practice")