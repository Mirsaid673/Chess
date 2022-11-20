cmake_minimum_required(VERSION 3.10)
set(PROJECT_NAME openGL)
project(${PROJECT_NAME})

set(EXECUTABLE out)
set(CXX_STANDARD 17)
set(LIBRARIES GL GLEW glfw)
file(GLOB SOURCES "Engine/*/*.cpp" "Engine/*/*/*.cpp" "Engine/*/*.h" "Engine/*/*/*.h")
set(SOURCES ${SOURCES} main.cpp chess/logic.cpp chess/piece.cpp chess/available_cells.cpp chess/ui.cpp chess/piece_choose_button.cpp)
file(GLOB INCLUDE_DIR "Engine/Vendor/*" "Engine/*/" ${CMAKE_SOURCE_DIR}/Engine)

find_package(spdlog REQUIRED)

add_executable(${EXECUTABLE} ${SOURCES})
set_target_properties(${EXECUTABLE} PROPERTIES CXX_STANDARD ${CXX_STANDARD})
target_link_libraries(${EXECUTABLE} ${LIBRARIES} spdlog::spdlog)
target_include_directories(${EXECUTABLE} PUBLIC ${INCLUDE_DIR})