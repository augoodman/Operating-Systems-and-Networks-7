cmake_minimum_required(VERSION 3.17)
project(Module7 C)

set(CMAKE_C_STANDARD 11)

add_executable(Module7
        User.c
        GoodmanInstanceHost.c
        GoodmanLoadBalancer.c
        InstanceHost.h
        LoadBalancer.h
        )

target_link_libraries(Module7 pthread)