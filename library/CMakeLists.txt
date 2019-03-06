set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(
	file_copier_srcs
	files_finder.h
	files_finder.cpp
	files_copier.h
	files_copier.cpp
)
add_library(file_copier_lib STATIC ${file_copier_srcs})
set_target_properties(file_copier_lib PROPERTIES LINKER_LANGUAGE CXX)

if(ENABLE_COVERAGE)
	target_link_libraries(file_copier_lib --coverage)
endif()

target_include_directories(file_copier_lib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})