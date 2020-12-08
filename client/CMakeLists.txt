cmake_minimum_required( VERSION  3.10 )


set( CMAKE_INCLUDE_CURRENT_DIR  ON )
set( CMAKE_CXX_STANDARD  17 )


project( asio_client )
set( LIB_NAME  asio_client_lib )


set( Boost_USE_STATIC_LIBS OFF )
set( Boost_USE_MULTITHREAD ON )
find_package( Boost 1.71.0 REQUIRED COMPONENTS system thread regex)
if( Boost_FOUND )
  include_directories( ${Boost_INCLUDE_DIRS} )
  link_directories( ${Boost_LIBRARY_DIRS} )
endif( Boost_FOUND )

set( USED_LIBS ${Boost_SYSTEM_LIBRARY} ${Boost_THREAD_LIBRARY} ${Boost_REGEX_LIBRARY})


file( GLOB_RECURSE  HEADERS  "*.hpp"  "*.h" )
file( GLOB_RECURSE  SOURCES  "*.cpp"  "*.c" !"main.cpp" )


add_library( ${LIB_NAME}  ${HEADERS}  ${SOURCES} )

add_executable( ${PROJECT_NAME} main.cpp )
target_link_libraries( ${PROJECT_NAME} ${LIB_NAME} )
target_link_libraries( ${PROJECT_NAME} ${USED_LIBS})
