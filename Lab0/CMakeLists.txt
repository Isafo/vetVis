cmake_minimum_required(VERSION 2.8)
project(Lab0)

add_executable(Test1 main1.cpp)

ADD_LIBRARY(Vec3f Vec3f.h Vec3f.cpp)
TARGET_LINK_LIBRARIES(Test1 Vec3f)