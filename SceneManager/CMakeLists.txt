add_library(SceneManager STATIC)

target_sources(SceneManager
    PRIVATE
        src/scene_manager.cpp
    PUBLIC
        FILE_SET HEADERS
        BASE_DIRS include
        FILES
            include/scene_manager.hpp
            include/scene.hpp
)

target_link_libraries(SceneManager PRIVATE raylib)