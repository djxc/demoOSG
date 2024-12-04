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
include_directories("D:\\code\\c\\osg-3.7.0\\include")
include_directories("D:\\Qt\\Qt5.14.2\\5.14.2\\msvc2017_64\\include")

set(LIB_OSG_PATH "D:\\code\\c\\osg-3.7.0\\lib")

find_package(Qt5 COMPONENTS Widgets REQUIRED)

add_subdirectory(src/handlers)
add_subdirectory(src/callback)
add_subdirectory(src/userScene)
add_subdirectory(src/precipitations)
add_subdirectory(src/main)

file(GLOB_RECURSE srcs CONFIGURE_DEPENDS src/*.cpp src/*.h)
add_executable(demoOSG ${srcs})

# 除了依赖第三方库还需要添加子模块
target_link_libraries(demoOSG PRIVATE Qt5::Widgets
    ${LIB_OSG_PATH}\\osgd.lib
    ${LIB_OSG_PATH}\\osgGAd.lib
    ${LIB_OSG_PATH}\\osgTextd.lib
    ${LIB_OSG_PATH}\\osgUtild.lib
    ${LIB_OSG_PATH}\\osgViewerd.lib
    ${LIB_OSG_PATH}\\osgDBd.lib
    ${LIB_OSG_PATH}\\osgManipulatord.lib
    ${LIB_OSG_PATH}\\OpenThreadsd.lib
    ${LIB_OSG_PATH}\\osgWidgetd.lib
    ${LIB_OSG_PATH}\\osgVolumed.lib
    ${LIB_OSG_PATH}\\osgUId.lib
    ${LIB_OSG_PATH}\\osgSimd.lib
    ${LIB_OSG_PATH}\\osgPresentationd.lib
    ${LIB_OSG_PATH}\\osgParticled.lib
    ${LIB_OSG_PATH}\\osgFXd.lib
    ${LIB_OSG_PATH}\\osgQOpenGLd.lib
    main callback userScene handlers precipitations)
