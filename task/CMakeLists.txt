cmake_minimum_required(VERSION 3.16)

project(VFS)

set(CMAKE_CXX_STANDARD 17)

include_directories(VFS/FS/include)
include_directories(VFS/ManagerMetaFileInfo/include)
add_executable( vfs
                main.cpp
                VFS/FS/src/VFS.cpp
                VFS/ManagerMetaFileInfo/src/ManagerMeta.cpp)           