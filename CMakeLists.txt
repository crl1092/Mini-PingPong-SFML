cmake_minimum_required(VERSION 4.3)
project(PingPong)

set(CMAKE_CXX_STANDARD 20)
find_package(SFML 3.0 REQUIRED COMPONENTS Graphics Window System Audio)

add_executable(PingPong src/main.cpp)

target_link_libraries(${PROJECT_NAME} PRIVATE SFML::Graphics SFML::Window SFML::System SFML::Audio)