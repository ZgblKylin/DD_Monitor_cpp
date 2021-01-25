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
find_package(Qt5
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

target_precompile_headers(${PROJECT_NAME} PRIVATE src/common.hpp)

target_include_directories(${PROJECT_NAME} PRIVATE include)

target_sources(${PROJECT_NAME}
  PRIVATE
    src/common.hpp
    src/common.cpp

    src/VideoCache.hpp
    src/VideoCache.cpp

    src/VideoPlayer.hpp
    src/VideoPlayer.cpp

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
# OpenSSL
find_package(OpenSSL REQUIRED)
target_link_libraries(${PROJECT_NAME} PRIVATE OpenSSL::SSL OpenSSL::Crypto)

# KtUtils
add_subdirectory(third_party/KtUtils)
target_link_libraries(${PROJECT_NAME} PRIVATE KtUtils)

# vlc-qt
include(third_party/vlc-qt/vlc-qt.cmake)
target_link_libraries(${PROJECT_NAME}
  PRIVATE
    VLCQt::Core
    VLCQt::Widgets
)
