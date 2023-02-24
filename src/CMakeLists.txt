project(xftps CXX)
cmake_minimum_required(VERSION 3.2)
set(CMAKE_CXX_STANDARD 14)
add_executable(xftps ev_ftp_server.cpp
        factory.cpp
        fcmd.cpp
        flist.cpp
        fport.cpp
        fretr.cpp
        fstore.cpp
        ftask.cpp
        fuser.cpp
        thread_pool.cpp
        thread.cpp fdel.cpp)

include_directories(include /usr/local/include)
link_directories(/usr/local/lib)
target_link_libraries(xftps event pthread)