cmake_minimum_required(VERSION 3.5)

project(demoOSG LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 添加子模块目录到CMAKE_MODULE_PATH，以便找到各个模块的CMakeLists.txt文件
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${PROJECT_SOURCE_DIR}/src)

# 引入第三方头文件
include_directories("D:\\code\\c\\osg\\include")

set(LIB_OSG_PATH "D:\\code\\c\\osg\\lib")

find_package(Qt5 COMPONENTS Widgets REQUIRED)

add_subdirectory(src/callback)
add_subdirectory(src/userScene)
add_subdirectory(src/main)

file(GLOB_RECURSE srcs CONFIGURE_DEPENDS src/*.cpp src/*.h)
add_executable(demoOSG ${srcs})

# 除了依赖第三方库还需要添加子模块
target_link_libraries(demoOSG PRIVATE Qt5::Widgets
    ${LIB_OSG_PATH}\\osg.lib
    ${LIB_OSG_PATH}\\osgGA.lib
    ${LIB_OSG_PATH}\\osgText.lib
    ${LIB_OSG_PATH}\\osgUtil.lib
    ${LIB_OSG_PATH}\\osgViewer.lib
    ${LIB_OSG_PATH}\\osgDB.lib
    ${LIB_OSG_PATH}\\OpenThreads.lib
    callback userScene)
