cmake_minimum_required(VERSION 3.3)
#set(CMAKE_CONFIGURATION_TYPES "Release" CACHE STRING "" FORCE)
set(CMAKE_GENERATOR_PLATFORM "Win32" CACHE STRING "" FORCE)

project(MyMesh VERSION 1.0.0)

add_executable(MyMesh 
	main.cpp
)

set(openmesh_DIR "C:/repos/OpenMesh-9.0.0/build")

# OPENMESH_FOUND			- system has OPENMESH
# OPENMESH_INCLUDE_DIR		- the OPENMESH include directory
# OPENMESH_LIBRARIES		- the OPENMESH libraries
# OPENMESH_LIBRARY_DIR		- the OPENMESH libraries directory

find_package(openmesh REQUIRED)
    target_include_directories(MyMesh PUBLIC ${OPENMESH_INCLUDE_DIRS})
    target_link_libraries(MyMesh PUBLIC ${OPENMESH_LIBRARIES})


