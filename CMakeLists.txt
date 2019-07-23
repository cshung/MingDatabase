project(MingDatabase CXX)
add_definitions(-std=c++11)
set (SRCS allocator_layer.cpp caching_layer.cpp program.cpp btree.cpp file_layer.cpp)
add_executable (MingDatabase ${SRCS})
