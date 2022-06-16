cmake_minimum_required(VERSION 3.16)
project (Test)
set(CMAKE_CXX_FLAGS	" -std=c++11 -pthread -m64 -Wno-write-strings -fexceptions  -fno-inline -rdynamic -fsanitize=address -fsanitize-recover=all ")
#set(CMAKE_CXX_STANDARD 11)
add_executable(test
        data_structure/heap.h
        data_structure/rbtree.h
        data_structure/string.h
        data_structure/string.cpp
        data_structure/vector.h
        huffman/encoder.cpp
        huffman/encoder.h
        huffman/decoder.cpp
        huffman/decoder.h
        basicClass.cpp
        basicClass.h
        clock.h
        clock.cpp
        customEvent.cpp
        customEvent.h
        customObject.cpp
        customObject.h
        global.h
        graph.cpp
        graph.h
        identity.cpp
        identity.h
        main.cpp
        server.cpp
        server.h
        sha256.cpp
        sha256.h
        timeHeap.h)
