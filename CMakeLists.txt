cmake_minimum_required(VERSION 4.2)
project(low_level_nlp C)

set(CMAKE_C_STANDARD 11)

include_directories(include)

add_executable(low_level_nlp
        src/00_Main.c
        src/01_Reading.c
        src/02_Node-STR.c
        src/03_Dijkstra.c
        src/04_Outputs.c
)