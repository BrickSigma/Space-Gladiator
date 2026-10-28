add_library(SceneManager STATIC)

if(MSVC)
  target_compile_options(SceneManager PRIVATE /W4 /WX)
else()
  target_compile_options(SceneManager PRIVATE -Wall -Wextra -Wpedantic -Werror)
endif()

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