cmake_minimum_required(VERSION 3.22)
project(game
		VERSION "0.0.1"
		DESCRIPTION "fool around with opengl tbh"
		LANGUAGES C CXX
)

set(ENABLE_TESTS FALSE)

include(FetchContent)

if (${ENABLE_TESTS})
	message("Tests are enabled!")
	FetchContent_Declare(
		googletest
		GIT_REPOSITORY https://github.com/google/googletest.git
		GIT_TAG v1.15.2
	)

	FetchContent_GetProperties(googletest)

	set(FETCHCONTENT_QUIET FALSE)
endif()

FetchContent_Declare(
	stb_lib
	GIT_REPOSITORY https://github.com/nothings/stb.git
	GIT_TAG 5c205738c191bcb0abc65c4febfa9bd25ff35234
)
FetchContent_MakeAvailable(stb_lib)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_subdirectory(source)

if (${ENABLE_TESTS})
	enable_testing()
	include(CTest)
	add_subdirectory(tests)
endif()
