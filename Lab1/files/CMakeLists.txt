cmake_minimum_required(VERSION 2.8)
project(Lab1)

set(VTK_DIR "H:/VTK/lib/cmake/vtk-6.2")
mark_as_advanced(VTK_DIR)
find_package(VTK REQUIRED)


include(${VTK_USE_FILE})

add_executable(Task1 Cone.cpp)
add_executable(Task2 Read.cpp)
add_executable(Task3 Write.cpp ImpFunc.h ImpFunc.cpp)

target_link_libraries(Task1 ${VTK_LIBRARIES})
target_link_libraries(Task2 ${VTK_LIBRARIES})
target_link_libraries(Task3 ${VTK_LIBRARIES})
