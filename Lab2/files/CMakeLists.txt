cmake_minimum_required(VERSION 2.8)
project(Lab2)

set(VTK_DIR "H:/VTK/lib/cmake/vtk-6.2")
mark_as_advanced(VTK_DIR)
find_package(VTK REQUIRED)


include(${VTK_USE_FILE})

add_executable(Lab2 TraceReader.cpp TraceReader.h Lab2.cpp)

target_link_libraries(Lab2 ${VTK_LIBRARIES})

