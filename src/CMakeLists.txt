
list(
        APPEND
        SRC_FILE
        bvhtree.cpp
        renderer.cpp
        scene.cpp
)
add_library(rt_core ${SRC_FILE})
target_include_directories(rt_core PUBLIC ${GLM_INCLUDE_DIR})