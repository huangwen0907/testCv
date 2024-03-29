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

add_executable(testopencv testopencv.cpp )
target_link_libraries( testopencv ${OpenCV_LIBS} ${PCL_LIBRARIES} )


add_executable(testimg testimg.cpp )
target_link_libraries( testimg ${OpenCV_LIBS} ${PCL_LIBRARIES} )

add_executable(testimgproc testimgproc.cpp )
target_link_libraries( testimgproc ${OpenCV_LIBS} ${PCL_LIBRARIES} )


add_executable(testCanny testCanny.cpp )
target_link_libraries( testCanny ${OpenCV_LIBS} ${PCL_LIBRARIES} )


add_executable(testHough testHough.cpp )
target_link_libraries( testHough ${OpenCV_LIBS} ${PCL_LIBRARIES} )

add_executable(testHistogram testHistogram.cpp )
target_link_libraries(testHistogram ${OpenCV_LIBS} ${PCL_LIBRARIES} )

add_executable(testRotate testRotate.cpp )
target_link_libraries(testRotate ${OpenCV_LIBS} ${PCL_LIBRARIES} )
