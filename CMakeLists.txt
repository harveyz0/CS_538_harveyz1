cmake_minimum_required(VERSION 3.5)
project( Basic_CS_538 )

set(CMAKE_INSTALL_PREFIX .)

set(MASTER_DEPEND "E:/Dependencies" CACHE PATH "Set this if necessary to your directory for dependencies.")	
set(CMAKE_PREFIX_PATH ${MASTER_DEPEND} ${CMAKE_PREFIX_PATH})
set(CMAKE_BUILD_TYPE Debug)


#####################################
# Require C++17
#####################################

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(OpenGL_GL_PREFERENCE GLVND)

#####################################
# Find necessary libraries
#####################################

find_package(OpenGL REQUIRED)
find_package(Vulkan REQUIRED)
find_package(assimp REQUIRED)
find_package(glfw3 3.3 REQUIRED) 
find_package(GLEW REQUIRED)	

#add_definitions(-DGLEW_STATIC)

#####################################
# Get general sources
#####################################

include_directories("src/include" ${Vulkan_INCLUDE_DIR} ${ASSIMP_INCLUDE_DIRS} ${GLEW_INCLUDE_DIRS})

file(GLOB GENERAL_SOURCES    
    "src/lib/*.cpp"  
    "src/lib/*.hpp"  
)

#####################################
# Create executables 
# and install targets
#####################################

set(ALL_LIBRARIES ${Vulkan_LIBRARIES} ${ASSIMP_LIBRARIES} ${ASSIMP_ZLIB} glfw GLEW::glew ${GLEW_SHARED_LIBRARIES} ${OPENGL_LIBRARIES})

# RenderApp
#add_executable(BasicRenderApp ${GENERAL_SOURCES} "./src/app/BasicRenderApp.cpp")
#target_link_libraries(BasicRenderApp ${ALL_LIBRARIES})
#install(TARGETS BasicRenderApp RUNTIME DESTINATION bin/BasicRenderApp)
#install(DIRECTORY shaders/BasicRenderApp DESTINATION bin/BasicRenderApp/shaders)

# PExercise01
#add_executable(PExercise01 ${GENERAL_SOURCES} "./src/app/PExercise01.cpp")
#target_link_libraries(PExercise01 ${ALL_LIBRARIES})
#install(TARGETS PExercise01 RUNTIME DESTINATION bin/PExercise01)
#install(DIRECTORY shaders/PExercise01 DESTINATION bin/PExercise01/shaders)


#add_executable(Assign01 ${GENERAL_SOURCES} "./src/app/Assign01.cpp")
#target_link_libraries(Assign01 ${ALL_LIBRARIES})
#install(TARGETS Assign01 RUNTIME DESTINATION bin/Assign01)
#install(DIRECTORY shaders/Assign01 DESTINATION bin/Assign01/shaders)


add_executable(tester ${GENERAL_SOURCES} "./src/lib/tester.cpp")
target_link_libraries(tester ${ALL_LIBRARIES})
#install(TARGETS tester RUNTIME DESTINATION bin/tester)
#install(DIRECTORY shaders/tester DESTINATION bin/tester/shaders)
