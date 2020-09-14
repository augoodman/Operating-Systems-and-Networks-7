cmake_minimum_required(VERSION 3.17)
project(Module7 C)

set(CMAKE_C_STANDARD 11)

add_executable(Module7
        User.c
        GoodmanInstanceHost.c
        GoodmanLoadBalancer.c
        GoodmanInstanceHost.h
        GoodmanLoadBalancer.h
        )