cmake_minimum_required(VERSION 2.7)

# Name of the project
project(3dbilliards)

# Use glob to get the list of all source files.
file(GLOB_RECURSE SOURCES "src/*.cpp" "ext/glad/src/*.c")

# We don't really need to include header and resource files to build, but it's
# nice to have them show up in IDEs.
file(GLOB_RECURSE HEADERS "src/*.h" "ext/glad/*/*.h")
file(GLOB_RECURSE GLSL "resources/*.glsl")

include_directories("ext/glad/include")

# Set the executable.
add_executable(${CMAKE_PROJECT_NAME} ${SOURCES} ${HEADERS} ${GLSL})

SET(CMAKE_BUILD_TYPE Release)

# Add PhysX
if (APPLE)
    set(PHYSX_LIBRARIES_DIR /usr/local/PhysX-3.4/PhysX_3.4/Lib/osx64)
    set(PHYSX_SHARED_LIBRARIES_DIR /usr/local/PhysX-3.4/PxShared/lib/osx64)

    include_directories("/usr/local/PhysX-3.4/PhysX_3.4/Include")
    include_directories("/usr/local/PhysX-3.4/PxShared/include")
    target_link_libraries(${CMAKE_PROJECT_NAME} ${PHYSX_SHARED_LIBRARIES_DIR}/libPsFastXmlDEBUG.a)
    target_link_libraries(${CMAKE_PROJECT_NAME} ${PHYSX_SHARED_LIBRARIES_DIR}/libPxFoundationDEBUG.a)
    target_link_libraries(${CMAKE_PROJECT_NAME} ${PHYSX_SHARED_LIBRARIES_DIR}/libPxPvdSDKDEBUG.a)
    target_link_libraries(${CMAKE_PROJECT_NAME} ${PHYSX_SHARED_LIBRARIES_DIR}/libPxTaskDEBUG.a)

    #target_link_libraries(${CMAKE_PROJECT_NAME} ${PHYSX_LIBRARIES_DIR}/libLowLeasdfvelAABBDEBUG.a)

    target_link_libraries(${CMAKE_PROJECT_NAME} ${PHYSX_LIBRARIES_DIR}/libLowLevelAABBDEBUG.a)
    target_link_libraries(${CMAKE_PROJECT_NAME} ${PHYSX_LIBRARIES_DIR}/libLowLevelClothDEBUG.a)
    target_link_libraries(${CMAKE_PROJECT_NAME} ${PHYSX_LIBRARIES_DIR}/libLowLevelDEBUG.a)
    target_link_libraries(${CMAKE_PROJECT_NAME} ${PHYSX_LIBRARIES_DIR}/libLowLevelDynamicsDEBUG.a)
    target_link_libraries(${CMAKE_PROJECT_NAME} ${PHYSX_LIBRARIES_DIR}/libLowLevelParticlesDEBUG.a)
    target_link_libraries(${CMAKE_PROJECT_NAME} ${PHYSX_LIBRARIES_DIR}/libPhysX3CharacterKinematicDEBUG.a)
    target_link_libraries(${CMAKE_PROJECT_NAME} ${PHYSX_LIBRARIES_DIR}/libPhysX3CommonDEBUG.a)
    target_link_libraries(${CMAKE_PROJECT_NAME} ${PHYSX_LIBRARIES_DIR}/libPhysX3CookingDEBUG.a)
    target_link_libraries(${CMAKE_PROJECT_NAME} ${PHYSX_LIBRARIES_DIR}/libPhysX3DEBUG.a)
    target_link_libraries(${CMAKE_PROJECT_NAME} ${PHYSX_LIBRARIES_DIR}/libPhysX3ExtensionsDEBUG.a)
    target_link_libraries(${CMAKE_PROJECT_NAME} ${PHYSX_LIBRARIES_DIR}/libPhysX3VehicleDEBUG.a)
    target_link_libraries(${CMAKE_PROJECT_NAME} ${PHYSX_LIBRARIES_DIR}/libSceneQueryDEBUG.a)
    target_link_libraries(${CMAKE_PROJECT_NAME} ${PHYSX_LIBRARIES_DIR}/libSimulationControllerDEBUG.a)

elseif(UNIX)
  set(PHYSX_LIBRARIES_DIR ./ext/phyx34/Lib/linux64)
  set(PHYSX_BIN_DIR ./ext/phyx34/Bin/linux64)
  set(PHYSX_SHARED_LIBRARIES_DIR ./ext/PxShared/lib/linux64)
  set(PHYSX_SHARED_BIN_DIR ./ext/PxShared/bin/linux64)

  link_directories("./ext/phyx34/Lib/linux64" "./ext/phyx34/Bin/linux64"
          "./ext/PxShared/lib/linux64"
          "./ext/PxShared/bin/linux64")

  SET(PHSYX_DIRS ${PHYSX_LIBRARIES_DIR} ${PHYSX_BIN_DIR} ${PHYSX_SHARED_LIBRARIES_DIR} ${PHYSX_SHARED_BIN_DIR})
  #  link_directories( )
  #  link_directories( )

  include_directories("./ext/PxShared/include")
  include_directories("./ext/phyx34/Include")


  target_link_libraries(${CMAKE_PROJECT_NAME} ${CMAKE_HOME_DIRECTORY}/${PHYSX_SHARED_LIBRARIES_DIR}/libPsFastXmlDEBUG.a)
  target_link_libraries(${CMAKE_PROJECT_NAME} ${CMAKE_HOME_DIRECTORY}/${PHYSX_SHARED_BIN_DIR}/libPxFoundationDEBUG_x64.so)
  target_link_libraries(${CMAKE_PROJECT_NAME} ${CMAKE_HOME_DIRECTORY}/${PHYSX_SHARED_BIN_DIR}/libPxPvdSDKDEBUG_x64.so)
  target_link_libraries(${CMAKE_PROJECT_NAME} ${CMAKE_HOME_DIRECTORY}/${PHYSX_SHARED_LIBRARIES_DIR}/libPxTaskDEBUG.a)
  #
  ##target_link_libraries(${CMAKE_PROJECT_NAME} ${PHYSX_LIBRARIES_DIR}/libLowLeasdfvelAABBDEBUG.a)
  #
  target_link_libraries(${CMAKE_PROJECT_NAME} ${CMAKE_HOME_DIRECTORY}/${PHYSX_LIBRARIES_DIR}/libLowLevelAABBDEBUG.a)
  target_link_libraries(${CMAKE_PROJECT_NAME} ${CMAKE_HOME_DIRECTORY}/${PHYSX_LIBRARIES_DIR}/libLowLevelClothDEBUG.a)
  target_link_libraries(${CMAKE_PROJECT_NAME} ${CMAKE_HOME_DIRECTORY}/${PHYSX_LIBRARIES_DIR}/libLowLevelDEBUG.a)
  target_link_libraries(${CMAKE_PROJECT_NAME} ${CMAKE_HOME_DIRECTORY}/${PHYSX_LIBRARIES_DIR}/libLowLevelDynamicsDEBUG.a)
  target_link_libraries(${CMAKE_PROJECT_NAME} ${CMAKE_HOME_DIRECTORY}/${PHYSX_LIBRARIES_DIR}/libLowLevelParticlesDEBUG.a)
  target_link_libraries(${CMAKE_PROJECT_NAME} ${CMAKE_HOME_DIRECTORY}/${PHYSX_BIN_DIR}/libPhysX3CharacterKinematicDEBUG_x64.so)
  target_link_libraries(${CMAKE_PROJECT_NAME} ${CMAKE_HOME_DIRECTORY}/${PHYSX_BIN_DIR}/libPhysX3CommonDEBUG_x64.so)
  target_link_libraries(${CMAKE_PROJECT_NAME} ${CMAKE_HOME_DIRECTORY}/${PHYSX_BIN_DIR}/libPhysX3CookingDEBUG_x64.so)
  target_link_libraries(${CMAKE_PROJECT_NAME} ${CMAKE_HOME_DIRECTORY}/${PHYSX_BIN_DIR}/libPhysX3DEBUG_x64.so)
  target_link_libraries(${CMAKE_PROJECT_NAME} ${CMAKE_HOME_DIRECTORY}/${PHYSX_LIBRARIES_DIR}/libPhysX3ExtensionsDEBUG.a)
  target_link_libraries(${CMAKE_PROJECT_NAME} ${CMAKE_HOME_DIRECTORY}/${PHYSX_LIBRARIES_DIR}/libPhysX3VehicleDEBUG.a)
  target_link_libraries(${CMAKE_PROJECT_NAME} ${CMAKE_HOME_DIRECTORY}/${PHYSX_LIBRARIES_DIR}/libSceneQueryDEBUG.a)
  target_link_libraries(${CMAKE_PROJECT_NAME} ${CMAKE_HOME_DIRECTORY}/${PHYSX_LIBRARIES_DIR}/libSimulationControllerDEBUG.a)


endif()



# Add GLFW
# Get the GLFW environment variable.
# There should be a CMakeLists.txt in the specified directory.
set(GLFW_DIR "$ENV{GLFW_DIR}")
if(GLFW_DIR)
  message(STATUS "GLFW environment variable found")

  option(GLFW_BUILD_EXAMPLES "GLFW_BUILD_EXAMPLES" OFF)
  option(GLFW_BUILD_TESTS "GLFW_BUILD_TESTS" OFF)
  option(GLFW_BUILD_DOCS "GLFW_BUILD_DOCS" OFF)
  if(CMAKE_BUILD_TYPE MATCHES Release)
    add_subdirectory(${GLFW_DIR} ${GLFW_DIR}/release)
  else()
    add_subdirectory(${GLFW_DIR} ${GLFW_DIR}/debug)
  endif()

  include_directories(${GLFW_DIR}/include)
  target_link_libraries(${CMAKE_PROJECT_NAME} glfw ${GLFW_LIBRARIES})
else()
  message(STATUS "GLFW environment variable `GLFW_DIR` not found, GLFW3 must be installed with the system")

  find_package(PkgConfig)
  if (PKGCONFIG_FOUND)
    message(STATUS "PkgConfig found")
    pkg_search_module(GLFW REQUIRED glfw3)
    include_directories(${GLFW_INCLUDE_DIRS})
    target_link_libraries(${CMAKE_PROJECT_NAME} ${GLFW_LIBRARIES})
  else()
    message(STATUS "No PkgConfig found")
    find_package(glfw3 REQUIRED)
    include_directories(${GLFW_INCLUDE_DIRS})
    target_link_libraries(${CMAKE_PROJECT_NAME} glfw)
  endif()
endif()



# Add GLM
# Get the GLM environment variable. Since GLM is a header-only library, we
# just need to add it to the include directory.
set(GLM_INCLUDE_DIR "$ENV{GLM_INCLUDE_DIR}")
if(GLM_INCLUDE_DIR)
  include_directories(${GLM_INCLUDE_DIR})
  message(STATUS "GLM environment variable found")
else()
# If the GLM_INCLUDE_DIR environment variable is not set, we assume
# the user has installed GLM properly on their system
  message(STATUS "GLM environment variable `GLM_INCLUDE_DIR` not found, GLM must be installed with the system")
endif()



# OS specific options and libraries
if(WIN32)
  # c++0x is enabled by default.
  # -Wall produces way too many warnings.
  # -pedantic is not supported.
  target_link_libraries(${CMAKE_PROJECT_NAME} opengl32.lib)
else()
  # Enable all pedantic warnings.
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++0x -Wall -pedantic")

  if(APPLE)
    # Add required frameworks for GLFW.
    target_link_libraries(${CMAKE_PROJECT_NAME} "-framework OpenGL -framework Cocoa -framework IOKit -framework CoreVideo")
  else()
    #Link the Linux OpenGL library
    target_link_libraries(${CMAKE_PROJECT_NAME} "GL" "dl")
  endif()
endif()
