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

if(MSVC)
  target_compile_options(SceneManager PRIVATE /W4 /WX)
  else()
  target_compile_options(SceneManager PRIVATE -Wall -Wextra -Wpedantic -Werror)
endif()

if (ASAN_ENABLED)
   if(MSVC)
      target_compile_options(${PROJECT_NAME} PRIVATE /fsanitize=address /Zi)
   else()
      target_compile_options(${PROJECT_NAME} PRIVATE -fsanitize=address)
      target_link_libraries(${PROJECT_NAME} PRIVATE -fsanitize=address)
    endif()
endif()