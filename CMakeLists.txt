cmake_minimum_required(VERSION 3.22)
project(game
		VERSION "0.0.1"
		DESCRIPTION "fool around with opengl tbh"
		LANGUAGES C CXX
)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

file(GLOB Sources
	source/main.cpp
	source/exception.cpp
	source/window.cpp
)

file(GLOB Headers
	source/auto_release.h
	source/error.h
	source/exception.h
	source/window.h
	source/opengl.h
	third_party/opengl/wglext.h
	third_party/opengl/glext.h
)

add_executable(Game ${Sources} ${Headers})

target_link_libraries(Game PUBLIC opengl32.lib)
target_include_directories(Game PRIVATE third_party third_party/opengl)

target_compile_features(Game PUBLIC cxx_std_23)
target_compile_options(Game PUBLIC /W4 /WX /Zi /MP)
target_compile_definitions(Game PUBLIC -DNOMINMAX)

target_link_options(Game PRIVATE /INCREMENTAL)

target_include_directories(Game PUBLIC source)
 