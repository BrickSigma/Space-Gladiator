add_executable(${PROJECT_NAME})

target_sources(${PROJECT_NAME}
    PRIVATE
        src/main.cpp
        src/texture_manager.cpp
        src/scenes/start_scene.cpp
        src/scenes/game_scene.cpp
        src/scenes/credits_scene.cpp
        src/entities/entity.cpp
        src/entities/player.cpp
        src/entities/projectile.cpp
        src/entities/player_bullet.cpp
    PUBLIC
        FILE_SET HEADERS
        BASE_DIRS include
        FILES
        include/texture_manager.hpp
        include/scenes/start_scene.hpp
        include/scenes/game_scene.hpp
        include/scenes/credits_scene.hpp
        include/entities/entity.hpp
        include/entities/player.hpp
        include/entities/projectile.hpp
        include/entities/player_bullet.hpp
)

#set(raylib_VERBOSE 1)
target_link_libraries(${PROJECT_NAME} 
    PRIVATE
        Config
        SceneManager
        raylib
)

if(MSVC)
  target_compile_options(${PROJECT_NAME} PRIVATE /W4 /WX /fsanitize=address)
  target_link_libraries(${PROJECT_NAME} PRIVATE /fsanitize=address)
  else()
  target_compile_options(${PROJECT_NAME} PRIVATE -Wall -Wextra -Wpedantic -Werror -fsanitize=address -Wunused-variable)
  target_link_libraries(${PROJECT_NAME} PRIVATE -fsanitize=address)
endif()

# Web Configurations
if (${PLATFORM} STREQUAL "Web")
    set_target_properties(${PROJECT_NAME} PROPERTIES SUFFIX ".html") # Tell Emscripten to build an example.html file.
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -s USE_GLFW=3 -s ASSERTIONS=1 -s WASM=1 -s ASYNCIFY --preload-file assets")
endif()

# Checks if OSX and links appropriate frameworks (Only required on MacOS)
if (APPLE)
    target_link_libraries(${PROJECT_NAME} "-framework IOKit")
    target_link_libraries(${PROJECT_NAME} "-framework Cocoa")
    target_link_libraries(${PROJECT_NAME} "-framework OpenGL")
    target_link_libraries(${PROJECT_NAME} "-framework QuartzCore")
endif()

# Copy the assets folder for the game
add_custom_command(
    TARGET ${PROJECT_NAME}
    POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory 
        ${CMAKE_SOURCE_DIR}/assets
        ${CMAKE_CURRENT_BINARY_DIR}/assets
)