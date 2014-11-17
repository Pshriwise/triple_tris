cmake_minimum_required(VERSION 2.8)

#project name
project(dagmc_preproc)


#iMesh/moab
set(MOAB_INC ${MOAB_DIR}/include)
set(MOAB_LIB ${MOAB_DIR}/lib)

#set include flags
include_directories(${MOAB_INC})

#set lib flags
set(LIBRARIES ${MOAB_LIB}/libMOAB.so)

#set c++ flags
set(CMAKE_CXX_FLAGS -g)

set(CMAKE_INSTALL_PREFIX .)
add_executable(triple_triangles triple_triangles.cpp)
target_link_libraries(triple_triangles ${LIBRARIES})

install(TARGETS triple_triangles DESTINATION .)

