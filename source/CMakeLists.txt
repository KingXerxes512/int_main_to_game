file(GLOB Sources
	*.cpp
)

file(GLOB Shaders
	../assets/*.glsl
)

file(GLOB Headers
	*.h
)

file(GLOB opengl_headers
	../third_party/opengl/wglext.h
	../third_party/opengl/glext.h
)

add_executable(Game ${Sources} ${Headers} ${opengl_headers} ${Shaders})

target_link_libraries(Game PUBLIC opengl32.lib)
target_include_directories(Game PUBLIC source ../third_party ../third_party/opengl ${stb_lib_SOURCE_DIR})

target_compile_features(Game PUBLIC cxx_std_23)
target_compile_options(Game PUBLIC /W4 /WX /Zi /MP)
target_compile_definitions(Game PUBLIC -DNOMINMAX)

target_link_options(Game PRIVATE /INCREMENTAL)

source_group(Headers FILES ${Headers})
source_group(Headers/OpenGL FILES ${opengl_headers})
source_group(Shaders FILES ${Shaders})
source_group(Sources FILES ${Sources})

file(COPY_FILE ../Game.vcxproj.user  ../build/source/Game.vcxproj.user)
