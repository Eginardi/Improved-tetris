cmake_minimum_required(VERSION 3.0.0)
project(cmaketry VERSION 0.1.0)
find_package(SFML COMPONENTS system window graphics audio REQUIRED)

include(CTest)
enable_testing()

add_executable(cmaketry main.cpp)
target_link_libraries(cmaketry PRIVATE sfml-window sfml-audio sfml-graphics)


set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
