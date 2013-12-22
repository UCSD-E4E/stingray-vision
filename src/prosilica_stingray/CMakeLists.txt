cmake_minimum_required(VERSION 2.8.3)
project(prosilica_stingray)

## Find catkin macros and libraries
## if COMPONENTS list like find_package(catkin REQUIRED COMPONENTS xyz)
## is used, also find other catkin packages
find_package(catkin REQUIRED COMPONENTS prosilica_gige_sdk roscpp std_msgs sensor_msgs
             diagnostic_updater diagnostic_msgs image_transport self_test driver_base 
             dynamic_reconfigure camera_calibration_parsers polled_camera rosconsole)

## System dependencies are found with CMake's conventions
find_package(Boost REQUIRED COMPONENTS thread)

include_directories(include ${Boost_INCLUDE_DIR} ${catkin_INCLUDE_DIRS})

generate_dynamic_reconfigure_options(cfg/ProsilicaCamera.cfg)

## Uncomment this if the package has a setup.py. This macro ensures
## modules and global scripts declared therein get installed
## See http://ros.org/doc/api/catkin/html/user_guide/setup_dot_py.html
# catkin_python_setup()

################################################
## Declare ROS messages, services and actions ##
################################################

## To declare and build messages, services or actions from within this
## package, follow these steps:
## * Let MSG_DEP_SET be the set of packages whose message types you use in
##   your messages/services/actions (e.g. std_msgs, actionlib_msgs, ...).
## * In the file package.xml:
##   * add a build_depend and a run_depend tag for each package in MSG_DEP_SET
##   * If MSG_DEP_SET isn't empty the following dependencies might have been
##     pulled in transitively but can be declared for certainty nonetheless:
##     * add a build_depend tag for "message_generation"
##     * add a run_depend tag for "message_runtime"
## * In this file (CMakeLists.txt):
##   * add "message_generation" and every package in MSG_DEP_SET to
##     find_package(catkin REQUIRED COMPONENTS ...)
##   * add "message_runtime" and every package in MSG_DEP_SET to
##     catkin_package(CATKIN_DEPENDS ...)
##   * uncomment the add_*_files sections below as needed
##     and list every .msg/.srv/.action file to be processed
##   * uncomment the generate_messages entry below
##   * add every package in MSG_DEP_SET to generate_messages(DEPENDENCIES ...)

## Generate messages in the 'msg' folder
# add_message_files(
#   FILES
#   Message1.msg
#   Message2.msg
# )

## Generate services in the 'srv' folder
# add_service_files(
#   FILES
#   Service1.srv
#   Service2.srv
# )

## Generate actions in the 'action' folder
# add_action_files(
#   FILES
#   Action1.action
#   Action2.action
# )

## Generate added messages and services with any dependencies listed here
# generate_messages(
#   DEPENDENCIES
#   std_msgs  # Or other packages containing msgs
# )

###################################
## catkin specific configuration ##
###################################
## The catkin_package macro generates cmake config files for your package
## Declare things to be passed to dependent projects
## INCLUDE_DIRS: uncomment this if you package contains header files
## LIBRARIES: libraries you create in this project that dependent projects also need
## CATKIN_DEPENDS: catkin_packages dependent projects also need
## DEPENDS: system dependencies of this project that dependent projects also need
catkin_package(
  INCLUDE_DIRS include
#  LIBRARIES prosilica_stingray
#  CATKIN_DEPENDS other_catkin_pkg
#  DEPENDS system_lib
)

###########
## Build ##
###########

## Specify additional locations of header files
## Your package locations should be listed before other locations
include_directories(include)

## Declare a cpp library
add_library(prosilica
  src/libprosilica/prosilica.cpp
)
target_link_libraries(prosilica ${catkin_LIBRARIES})

## Declare a cpp executabe
add_executable(prosilica_node src/nodes/prosilica_node.cpp)
target_link_libraries(prosilica_node prosilica
  ${Boost_LIBRARIES}
  ${catkin_LIBRARIES}
)
add_dependencies(prosilica_node 
  ${catkin_EXPORTED_TARGETS}
  ${prosilica_camera_EXPORTED_TARGETS}
)


#add_executable(find_camera src/utilities/find_camera.cpp)


add_executable(read_memory src/utilities/read_memory.cpp)
target_link_libraries(read_memory prosilica ${catkin_LIBRARIES} ${Boost_LIBRARIES})

add_executable(write_memory src/utilities/write_memory.cpp)
target_link_libraries(write_memory prosilica ${catkin_LIBRARIES})

add_executable(set_inhibition src/utilities/set_inhibition.cpp)
target_link_libraries(set_inhibition prosilica ${catkin_LIBRARIES})

add_executable(set_ip src/utilities/set_ip.cpp)
target_link_libraries(set_ip prosilica ${catkin_LIBRARIES})

## Add cmake target dependencies of the executable/library
## as an example, message headers may need to be generated before nodes
# add_dependencies(prosilica_stingray_node prosilica_stingray_generate_messages_cpp)


#############
## Install ##
#############

# all install targets should use catkin DESTINATION variables
# See http://ros.org/doc/api/catkin/html/adv_user_guide/variables.html

## Mark executable scripts (Python etc.) for installation
## in contrast to setup.py, you can choose the destination
# install(PROGRAMS
#   scripts/my_python_script
#   DESTINATION ${CATKIN_PACKAGE_BIN_DESTINATION}
# )

## Mark executables and/or libraries for installation
install(TARGETS prosilica_node write_memory read_memory set_ip set_inhibition
    DESTINATION ${CATKIN_PACKAGE_BIN_DESTINATION}
)

install(TARGETS prosilica
    RUNTIME DESTINATION ${CATKIN_GLOBAL_BIN_DESTINATION}
    ARCHIVE DESTINATION ${CATKIN_PACKAGE_LIB_DESTINATION}
    LIBRARY DESTINATION ${CATKIN_PACKAGE_LIB_DESTINATION}
)

install(DIRECTORY launch
   DESTINATION ${CATKIN_PACKAGE_SHARE_DESTINATION}
)

install(FILES prosilica.launch streaming.launch
   DESTINATION ${CATKIN_PACKAGE_SHARE_DESTINATION}
)

## Mark cpp header files for installation
# install(DIRECTORY include/${PROJECT_NAME}/
#   DESTINATION ${CATKIN_PACKAGE_INCLUDE_DESTINATION}
#   FILES_MATCHING PATTERN "*.h"
#   PATTERN ".svn" EXCLUDE
# )

## Mark other files for installation (e.g. launch and bag files, etc.)
# install(FILES
#   # myfile1
#   # myfile2
#   DESTINATION ${CATKIN_PACKAGE_SHARE_DESTINATION}
# )

#############
## Testing ##
#############

## Add gtest based cpp test target and link libraries
# catkin_add_gtest(${PROJECT_NAME}-test test/test_prosilica_stingray.cpp)
# if(TARGET ${PROJECT_NAME}-test)
#   target_link_libraries(${PROJECT_NAME}-test ${PROJECT_NAME})
# endif()

## Add folders to be run by python nosetests
# catkin_add_nosetests(test)
