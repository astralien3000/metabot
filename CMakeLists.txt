cmake_minimum_required(VERSION 3.5)
project(metabot)

find_package(ament_cmake REQUIRED)
find_package(rclc REQUIRED)
find_package(custom_msgs REQUIRED)

find_package(rosidl_typesupport_test REQUIRED)

install(
  FILES
    config.h function.h kinematic.h leds.h  main.h  mapping.h motion.h motors.h
    function.cpp kinematic.cpp leds.cpp main.cpp mapping.cpp motion.cpp motors.cpp
  DESTINATION ${PROJECT_NAME}
)

ament_package_xml()

set(MAKEFILE_PATH "${CMAKE_INSTALL_PREFIX}/${PROJECT_NAME}/Makefile")
file(WRITE  "${MAKEFILE_PATH}" "APPLICATION = ${PROJECT_NAME}\n")
file(APPEND "${MAKEFILE_PATH}" "RIOTBASE ?= ${CMAKE_INSTALL_PREFIX}/RIOT\n")
file(APPEND "${MAKEFILE_PATH}" "BOARD = samr21-xpro-ceres\n")
file(APPEND "${MAKEFILE_PATH}" "QUIET ?= 0\n")
file(APPEND "${MAKEFILE_PATH}" "WERROR ?= 0\n")
foreach(dep ${${PROJECT_NAME}_BUILD_DEPENDS})
  foreach(idir ${${dep}_INCLUDE_DIRS})
    file(APPEND "${MAKEFILE_PATH}" "INCLUDES += -I${idir}\n")
  endforeach()
  file(APPEND "${MAKEFILE_PATH}" "DIRS += ${CMAKE_INSTALL_PREFIX}/${dep}\n")
  file(APPEND "${MAKEFILE_PATH}" "USEMODULE += ${dep}\n")
  file(APPEND "${MAKEFILE_PATH}" "include ${CMAKE_INSTALL_PREFIX}/${dep}/Makefile.include\n")
endforeach()
file(APPEND "${MAKEFILE_PATH}" "USEMODULE += xtimer\n")
file(APPEND "${MAKEFILE_PATH}" "USEMODULE += dynamixel\n")
file(APPEND "${MAKEFILE_PATH}" "include $(RIOTBASE)/Makefile.include\n")

ament_package()
