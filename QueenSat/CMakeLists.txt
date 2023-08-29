# CMakeList.txt : CMake project for CMakeProject2, include source and define
# project specific logic here.
#


# Add source to this project's executable.
add_executable (QueenSat "QueenSat.cpp" )

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET QueenSat PROPERTY CXX_STANDARD 20)
endif()

# Find package minisat
find_package(MiniSat CONFIG REQUIRED)
target_link_libraries(QueenSat PRIVATE MiniSat::libminisat)

