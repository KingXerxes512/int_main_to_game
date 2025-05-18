FetchContent_MakeAvailable(googletest)

include(GoogleTest)

mark_as_advanced(BUILD_GMOCK BUILD_GTEST gtest_hide_internal_symbols)

add_executable(unit_tests
	Vector3_Tests.cpp
	Matrix4_Tests.cpp
	../source/Matrix4.cpp
)

target_include_directories(unit_tests PUBLIC ../source)

target_compile_features(unit_tests PUBLIC cxx_std_23)
target_compile_options(unit_tests PUBLIC /W4 /WX /Zi /MP)
target_compile_definitions(unit_tests PUBLIC -DNOMINMAX)
target_link_options(unit_tests PRIVATE /INCREMENTAL)

target_link_libraries(unit_tests gmock_main)
gtest_discover_tests(unit_tests DISCOVERY_MODE PRE_TEST)
