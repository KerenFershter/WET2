cmake_minimum_required(VERSION 3.16)
project(Wet2)

set(CMAKE_CXX_STANDARD 14)

add_executable(Wet2 main.cpp Player.h Player.cpp Group.cpp Group.h UnionFind.h DynamicArray.h AvlTree.h Utils.h GameManager.cpp GameManager.h library.h)