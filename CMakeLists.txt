cmake_minimum_required(VERSION 3.0)

project(GOE)

# Set the C++ standard
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
# Add source files
file(GLOB SOURCES "src/*.cpp")
# Generate the executable
add_executable(${PROJECT_NAME} ${SOURCES})
# Add all the headers directory
target_include_directories(${PROJECT_NAME} PRIVATE include 
                            "C:/Program Files/SDL2-devel-2.28.1-VC/include"                                
                            "C:/Program Files/Relaxed-Lenses/include"                        
                            )

# Add the SDL libraries
target_link_directories(${PROJECT_NAME} PRIVATE "C:/Program Files/SDL2-devel-2.28.1-VC/lib/x64")
target_link_directories(${PROJECT_NAME} PRIVATE "C:/Program Files/Relaxed-Lenses/lib")



# Link SDL2 libraries to your project
target_link_libraries(${PROJECT_NAME} PRIVATE Relaxed-Lenses
                      SDL2
                      SDL2main
                      )
