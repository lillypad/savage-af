cmake_minimum_required(VERSION 2.8.9)
project(savage-af)
set(CMAKE_BUILD_TYPE Release)
include_directories(include)
file(GLOB SOURCES "src/*.c")
add_library(savage-af SHARED ${SOURCES})
set_target_properties(
  savage-af PROPERTIES
  SOVERSION 1)
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -pthread -O2 -pipe -Wall -Werror -fPIC -shared")
install(TARGETS savage-af DESTINATION /usr/lib)
find_package(OpenSSL REQUIRED)
if (OPENSSL_FOUND)
  include_directories(${OPENSSL_INCLUDE_DIR})
  target_link_libraries(${PROJECT_NAME} ${OPENSSL_LIBRARIES})
endif (OPENSSL_FOUND)
