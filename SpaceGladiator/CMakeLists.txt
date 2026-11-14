add_executable(${PROJECT_NAME})

if(WIN32)
    set_target_properties(${PROJECT_NAME} PROPERTIES
        WIN32_EXECUTABLE $<NOT:$<CONFIG:Debug>>
    )
    if(MSVC)
        target_link_options(${PROJECT_NAME} PRIVATE
            $<$<NOT:$<CONFIG:Debug>>:/ENTRY:mainCRTStartup>
        )
    endif()
endif()

target_sources(${PROJECT_NAME}
    PRIVATE
        src/main.cpp
        src/scenes/start_scene.cpp
        src/scenes/game_scene.cpp
        src/scenes/credits_scene.cpp
        src/scenes/scene_manager.cpp
        src/entities/entity.cpp
        src/entities/player.cpp
        src/entities/projectile.cpp
        src/entities/player_bullet.cpp
        src/entities/asteroid.cpp
        src/level_manager/spawner.cpp
        src/level_manager/level_manager.cpp
        src/level_manager/level.cpp
        src/utils/camera_shake.cpp
        src/utils/texture_manager.cpp
        src/utils/animation.cpp
        src/entities/explosion.cpp
        src/entities/diagonal_flyer.cpp
        src/entities/sine_wave_flyer.cpp
        src/entities/spinning_laser.cpp
        src/entities/moving_laser.cpp
        src/entities/flyer_spawner.cpp
    PUBLIC
        FILE_SET HEADERS
        BASE_DIRS include
        FILES
        include/scenes/start_scene.hpp
        include/scenes/game_scene.hpp
        include/scenes/credits_scene.hpp
        include/scenes/scene.hpp
        include/scenes/scene_manager.hpp
        include/entities/entity.hpp
        include/entities/player.hpp
        include/entities/projectile.hpp
        include/entities/player_bullet.hpp
        include/entities/asteroid.hpp
        include/level_manager/spawner.hpp
        include/level_manager/level_manager.hpp
        include/level_manager/level.hpp
        include/utils/camera_shake.hpp
        include/utils/texture_manager.hpp
        include/utils/animation.hpp
        include/utils/constants.hpp
        include/entities/explosion.hpp
        include/entities/diagonal_flyer.hpp
        include/entities/sine_wave_flyer.hpp
        include/entities/spinning_laser.hpp
        include/entities/moving_laser.hpp
        include/entities/flyer_spawner.hpp
)

#set(raylib_VERBOSE 1)
target_link_libraries(${PROJECT_NAME} 
    PRIVATE
        raylib
)

# Set build type as a compile definition
target_compile_definitions(${PROJECT_NAME} PRIVATE $<$<CONFIG:Debug>:DEBUG>)

# Set build flags as well
if(NOT MSVC)
    target_compile_options(${PROJECT_NAME} PRIVATE
        $<$<CONFIG:Debug>:-g -O0>
        $<$<NOT:$<CONFIG:Debug>>:-O2>
    )
endif()

if(MSVC)
  target_compile_options(${PROJECT_NAME} PRIVATE /W4 /WX)
else()
  target_compile_options(${PROJECT_NAME} PRIVATE
    -Wall -Wextra -Wpedantic -Werror
    -Wconversion
    -Wsign-conversion
    -Wshadow
    -Wdouble-promotion
    -Wunused-variable
  )
endif()

if (ASAN_ENABLED)
    if(MSVC)
      target_compile_options(${PROJECT_NAME} PRIVATE /fsanitize=address /Zi)
    elseif(NOT (${PLATFORM} STREQUAL "Web"))
      target_compile_options(${PROJECT_NAME} PRIVATE -fsanitize=address)
      target_link_libraries(${PROJECT_NAME} PRIVATE -fsanitize=address)
    endif()
endif()

# Web Configurations
if (${PLATFORM} STREQUAL "Web")
    set_target_properties(${PROJECT_NAME} PROPERTIES SUFFIX ".html") # Tell Emscripten to build an example.html file.
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -s USE_GLFW=3 -s ASSERTIONS=1 -s WASM=1 -s ASYNCIFY --preload-file ${CMAKE_SOURCE_DIR}/assets@/assets -sABORTING_MALLOC=0")
    set_target_properties(${PROJECT_NAME} PROPERTIES ADDITIONAL_CLEAN_FILES "${PROJECT_NAME}.data;${PROJECT_NAME}.wasm;${PROJECT_NAME}.js")
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