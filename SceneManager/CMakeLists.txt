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

if(MSVC)
  target_compile_options(SceneManager PRIVATE /W4 /WX /fsanitize=address)
  target_link_libraries(SceneManager PRIVATE raylib /fsanitize=address)
  else()
  target_compile_options(SceneManager PRIVATE -Wall -Wextra -Wpedantic -Werror -fsanitize=address)
  target_link_libraries(SceneManager PRIVATE raylib -fsanitize=address)
endif()