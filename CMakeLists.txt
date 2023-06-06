
cmake_minimum_required(VERSION 3.12)

project(smart_sensor VERSION 0.1.0 DESCRIPTION "My Smart Sensor Project")

include(CTest)

find_library(GXIAPI_LIB NAMES gxiapi PATHS ${CMAKE_CURRENT_SOURCE_DIR}/daheng/lib/armv7hf)
find_package(OpenCV REQUIRED)
find_package(OpenMP REQUIRED)

add_executable(smart_sensor main.cpp)

# Add the directories containing the header files
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/daheng/inc)

target_link_libraries(smart_sensor
  PRIVATE ${GXIAPI_LIB}
  PUBLIC ${OpenCV_LIBS}
         OpenMP::OpenMP_CXX
)

set_target_properties(smart_sensor PROPERTIES
  CXX_STANDARD 11
  CXX_STANDARD_REQUIRED ON
  VERSION ${PROJECT_VERSION}
  SOVERSION ${PROJECT_VERSION_MAJOR}
  DESCRIPTION ${PROJECT_DESCRIPTION}
)

include(CPack)
