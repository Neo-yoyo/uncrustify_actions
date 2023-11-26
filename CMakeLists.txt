cmake_minimum_required(VERSION 3.12)
project(json)

#активируйте как минимум 17-й стандарт
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_package(format GIT_REPOSITORY git@github.com:QCoreTech/format.cmake.git GIT_TAG master)
add_package(xt-variant GIT_REPOSITORY git@github.com:QCoreTech/xt-variant.git GIT_TAG dev)

add_executable(json
                main.cpp
                json.h
                json.cpp)

# подключите нужные библиотеки, поставив флаги -ltbb и -lpthread
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -ltbb -lpthread")
