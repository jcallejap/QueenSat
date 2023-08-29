# CMakeList.txt : CMake project for CMakeProject2, include source and define
# project specific logic here.
#
# https://github.com/stp/stp/blob/master/cmake/modules/Findminisat.cmake


# Add source to this project's executable.
add_executable (CMakeProject2 "QueenSat.cpp" )

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET CMakeProject2 PROPERTY CXX_STANDARD 20)
endif()

# Find package minisat
find_package(MiniSat CONFIG REQUIRED)
target_link_libraries(CMakeProject2 PRIVATE MiniSat::libminisat)

