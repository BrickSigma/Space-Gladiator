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

target_link_libraries(SceneManager PRIVATE raylib Config)

# Set build flags as well
if(NOT MSVC)
    target_compile_options(SceneManager PRIVATE
        $<$<CONFIG:Debug>:-g -O0>
        $<$<NOT:$<CONFIG:Debug>>:-O2>
    )
endif()

if(MSVC)
  target_compile_options(SceneManager PRIVATE /W4 /WX)
else()
  target_compile_options(SceneManager PRIVATE
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
      target_compile_options(SceneManager PRIVATE /fsanitize=address /Zi)
   elseif(NOT (${PLATFORM} STREQUAL "Web"))
      target_compile_options(SceneManager PRIVATE -fsanitize=address)
      target_link_libraries(SceneManager PRIVATE -fsanitize=address)
    endif()
endif()