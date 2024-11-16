cmake_minimum_required(VERSION 3.10)
project(Translator CXX)

set(CMAKE_CXX_STANDARD 20)
set(SOURCES 
	src/main.cpp
	src/hash_table.cpp
)
set(HEADERS
	include/hash_table.hpp
)

include_directories("include/")


add_executable(Translator 
	${SOURCES}
	${HEDERS}
)
