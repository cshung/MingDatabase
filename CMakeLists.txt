project(MingDatabase CXX)
add_definitions(-std=c++11)
set (SRCS page_file.cpp page_allocator.cpp program.cpp)
add_executable (MingDatabase ${SRCS})
