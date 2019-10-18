cmake_minimum_required( VERSION 2.8 )
project( joinMap )

set( CMAKE_BUILD_TYPE Release )
set( CMAKE_CXX_FLAGS "-std=c++11 -O3" )

# opencv
find_package( OpenCV REQUIRED )
include_directories( ${OpenCV_INCLUDE_DIRS} )

# eigen
include_directories( "/usr/include/eigen3/" )

# pcl
find_package( PCL REQUIRED COMPONENT common io )
include_directories( ${PCL_INCLUDE_DIRS} )
add_definitions( ${PCL_DEFINITIONS} )

add_executable(testVedio testVedio.cpp )
target_link_libraries( testVedio ${OpenCV_LIBS} ${PCL_LIBRARIES} )

add_executable(testFeatureHarris testFeatureHarris.cpp )
target_link_libraries( testFeatureHarris ${OpenCV_LIBS} ${PCL_LIBRARIES} )


add_executable(testFeatureDescription testFeatureDescription.cpp )
target_link_libraries( testFeatureDescription ${OpenCV_LIBS} ${PCL_LIBRARIES} )
