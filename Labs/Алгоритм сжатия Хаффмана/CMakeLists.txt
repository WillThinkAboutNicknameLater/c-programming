cmake_minimum_required(VERSION 3.5.1)
project(lab5 C)

set(SRC src/main.c src/BoolList.c src/Byte.c src/Character.c src/CodeTable.c src/Tree.c src/TreeList.c)

add_executable(${PROJECT_NAME} ${SRC})

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}/../modules")
include(common_lab)
