cmake_minimum_required(VERSION 3.16)
project (Test)
set(CMAKE_CXX_FLAGS	" -std=c++11 -m64 -Wno-write-strings -fexceptions  -fno-inline -rdynamic -fsanitize=address -fsanitize-recover=all  -fsanitize=leak")
#set(CMAKE_CXX_STANDARD 11)
add_executable(test
        data_structure/heap.h
        data_structure/heap.cpp
        data_structure/string.h
        data_structure/string.cpp
        data_structure/heap.h
        data_structure/heap.cpp
        data_structure/vector.h
        basicClass.cpp
        basicClass.h
        customEvent.cpp
        customEvent.h
        customObject.h
        global.h
        graph.cpp
        graph.h
        identity.h
        main.cpp
        server.cpp
        server.h)
