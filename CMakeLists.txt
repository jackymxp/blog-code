cmake_minimum_required(VERSION 3.10)

# 要卸载project之前

SET(CMAKE_C_COMPILER /usr/bin/gcc)
SET(CMAKE_CXX_COMPILER /usr/bin/clang)


project(C___CODE)
set(CMAKE_CXX_STANDARD 11)
SET(CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS} "-pthread")


# 递归遍历项目根目录下所有的 .cpp 文件
file (GLOB_RECURSE files ./src/*.cpp)

foreach (file ${files})
    string(REGEX REPLACE ".+/(.+)\\..*" "\\1" exe ${file})
    add_executable (${exe} ${file})
    message (\ \ \ \ --\ src/${exe}.cpp\ will\ be\ compiled\ to\ bin/${exe})

    set( CMAKE_INSTALL_PREFIX "${CMAKE_SOURCE_DIR}/bin/")
    install(TARGETS ${exe} RUNTIME DESTINATION ${CMAKE_INSTALL_PREFIX})
endforeach ()