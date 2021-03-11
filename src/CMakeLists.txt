
list(
        APPEND
        SRC_FILE
        bvhtree.cpp
        renderer.cpp
        scene.cpp
        model.cpp
)
add_library(rt_core ${SRC_FILE})
target_include_directories(rt_core PUBLIC ${GLM_INCLUDE_DIR} ${TINY_OBJ_LOADER_INCLUDE_DIR})
target_link_libraries(rt_core PRIVATE spdlog::spdlog)