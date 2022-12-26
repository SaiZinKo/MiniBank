cmake_minimum_required(VERSION 3.23)
project(MiniBank)

set(CMAKE_CXX_STANDARD 14)

add_executable(MiniBank main.cpp bank.cpp bank.h model/user.h data-store.cpp data-store.h common-utils.cpp common-utils.h)
