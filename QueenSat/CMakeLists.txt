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

#####################################################################


# -----------------------------------------------------------------------------
# Find Minisat
# -----------------------------------------------------------------------------
# find_package(minisat)
# set(MINISAT_INCLUDE_DIRS "" CACHE PATH "MiniSat include directory")
# set(MINISAT_LIBDIR "" CACHE PATH "MiniSat library directory")
# if (NOT MINISAT_FOUND)
#   message(FATAL_ERROR "You must install minisat from https://github.com/niklasso/minisat")
# else()
#   message(STATUS "OK, found Minisat library under ${MINISAT_LIBRARIES} and Minisat include dirs under ${MINISAT_INCLUDE_DIR}")
# endif()
# include_directories(${MINISAT_INCLUDE_DIR})
# set(LIBS ${LIBS} ${MINISAT_LIBRARIES})







#####################################################################
# 
# message(STATUS "Finding minisat headers...")
# message(STATUS "looking at:  ${MINISAT_INCLUDE_DIRS}")
# find_path(MINISAT_INCLUDE_DIR minisat/core/Solver.h
#           HINTS ${MINISAT_INCLUDE_DIRS}
#           PATH_SUFFIXES minisat minisat2 )
# message(STATUS "found: MINISAT_INCLUDE_DIR: ${MINISAT_INCLUDE_DIR}")
# 
# message(STATUS "Finding minisat libs...")
# message(STATUS "looking at:  ${MINISAT_LIBDIR}")
# find_library(MINISAT_LIBRARY NAMES minisat minisat2
#          HINTS ${MINISAT_LIBDIR} ${MINISAT_LIBDIR}/lib ${MINISAT_LIBRARY_DIRS} )
# message(STATUS "found: MINISAT_LIBRARY: ${MINISAT_LIBRARY}")
# 
# set(MINISAT_LIBRARIES ${MINISAT_LIBRARY} )
# set(MINISAT_INCLUDE_DIRS ${MINISAT_INCLUDE_DIR} )
# 
# include(FindPackageHandleStandardArgs)
# # handle the QUIETLY and REQUIRED arguments and set MINISAT_FOUND to TRUE
# # if all listed variables are set
# find_package_handle_standard_args(minisat  DEFAULT_MSG MINISAT_LIBRARY MINISAT_INCLUDE_DIR)
# 
# mark_as_advanced(MINISAT_INCLUDE_DIR MINISAT_LIBRARY )
