cmake_minimum_required(VERSION 2.8)
project(Lab0)

add_executable(Test1 main1.cpp)

ADD_LIBRARY(Vec3f Vec3f.h Vec3f.cpp)
TARGET_LINK_LIBRARIES(Test1 Vec3f)

SET(LIB2_SOURCE Container.h Container.cpp Object.h String.h Float.h)
ADD_LIBRARY(Lib2 ${LIB2_SOURCE})
ADD_EXECUTABLE(Test2 main2.cpp)
TARGET_LINK_LIBRARIES(Test2 Lib2)