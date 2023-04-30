cmake_minimum_required(VERSION 3.13.4)

###############################################
# Set project_name as the directory name
###############################################

file(RELATIVE_PATH project_name ${CMAKE_CURRENT_LIST_DIR}/.. ${CMAKE_CURRENT_LIST_DIR})

SET(EXE_NAME "clearc")

project (${project_name})

###############################################
# Generate lexer and parser codes
###############################################

message("-- Generating lexer and parser codes")
execute_process(WORKING_DIRECTORY ${CMAKE_CURRENT_LIST_DIR}/src
				COMMAND flex Lexer.l
				COMMAND bison -d Parser.y)

###############################################
# Set dependencies
###############################################

# Set project compile flags.
set (CMAKE_CXX_STANDARD 20)
#LDFLAGS="-L/opt/homebrew/opt/llvm@14/lib/c++ -Wl,-rpath,/opt/homebrew/opt/llvm@14/lib/c++"
#echo 'export PATH="/opt/homebrew/opt/llvm@14/bin:$PATH"' >> ~/.zshrc
#export LDFLAGS="-L/opt/homebrew/opt/llvm@14/lib"
#export CPPFLAGS="-I/opt/homebrew/opt/llvm@14/include"
find_package(LLVM REQUIRED CONFIG)
message(STATUS "Found LLVM ${LLVM_PACKAGE_VERSION}")
message(STATUS "Using LLVMConfig.cmake in: ${LLVM_DIR}")

include_directories(${LLVM_INCLUDE_DIRS})

separate_arguments(LLVM_DEFINITIONS_LIST NATIVE_COMMAND ${LLVM_DEFINITIONS})

add_definitions(${LLVM_DEFINITIONS})

include_directories(${CMAKE_SOURCE_DIR}/include)

###############################################
# Set project files
###############################################

file(GLOB SOURCES ./src/*.hpp ./src/*.cpp)

add_executable(${EXE_NAME} ${SOURCES})

###############################################
# Link
###############################################

if (WIN32)
	target_link_libraries(${project_name} ${LLVM_AVAILABLE_LIBS})
else()
	execute_process(COMMAND llvm-config --libs all
					OUTPUT_VARIABLE llvm_libs)
	string(STRIP ${llvm_libs} llvm_libs)
	string(REPLACE " " ";" llvm_libs ${llvm_libs})
	message(STATUS "lib: ${llvm_libs}")
	target_link_libraries(${EXE_NAME} ${llvm_libs})
endif()