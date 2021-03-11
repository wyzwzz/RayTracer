cmake_minimum_required(VERSION 3.17)
project(RayTracer CXX )



include_directories(include)


set(GLM_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/third_party/glm)


set(TINY_OBJ_LOADER_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/third_party/tinyobjloader)






add_subdirectory(src)

add_subdirectory(example)

add_subdirectory(third_party/spdlog)

add_subdirectory(third_party/glm)

add_subdirectory(third_party/googletest)

add_subdirectory(third_party/tinyobjloader)