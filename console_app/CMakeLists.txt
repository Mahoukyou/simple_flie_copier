set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(
	console_app_srcs
	main.cpp
)
add_executable(console_app ${console_app_srcs})
set_target_properties(console_app PROPERTIES LINKER_LANGUAGE CXX)
target_link_libraries(console_app file_copier_lib)

if(ENABLE_COVERAGE)
	target_link_libraries(console_app --coverage)
endif()

target_include_directories(console_app PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})