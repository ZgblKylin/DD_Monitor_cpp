# ================================================================
# Setup Project
# ================================================================
cmake_minimum_required(VERSION 3.16.0)
project(DDMonitor VERSION 0.0.1 LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_LIST_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_CURRENT_LIST_DIR}/lib)



# ================================================================
# Initialize Qt
# ================================================================
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)
find_package(Qt6
  COMPONENTS
    Core
    Gui
    Widgets
    Concurrent
    Svg
    Network
  REQUIRED
)



# ================================================================
# Initialize Target
# ================================================================
add_executable(${PROJECT_NAME} WIN32)

target_precompile_headers(${PROJECT_NAME} PRIVATE src/common.h)

target_sources(${PROJECT_NAME}
  PRIVATE
    src/common.h
    src/common.cpp

    src/main.cpp
)

target_link_libraries(${PROJECT_NAME}
  PRIVATE
    Qt::Core
    Qt::Gui
    Qt::Widgets
    Qt::Concurrent
    Qt::Network
)



# ================================================================
# Third Party
# ================================================================
add_subdirectory(third_party/KtUtils)
target_link_libraries(${PROJECT_NAME} PRIVATE KtUtils)
