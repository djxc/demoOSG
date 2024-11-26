cmake_minimum_required(VERSION 3.5)

project(demoOSG LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# QtCreator supports the following variables for Android, which are identical to qmake Android variables.
# Check http://doc.qt.io/qt-5/deployment-android.html for more information.
# They need to be set before the find_package(Qt5 ...) call.

#if(ANDROID)
#    set(ANDROID_PACKAGE_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/android")
#    if (ANDROID_ABI STREQUAL "armeabi-v7a")
#        set(ANDROID_EXTRA_LIBS
#            ${CMAKE_CURRENT_SOURCE_DIR}/path/to/libcrypto.so
#            ${CMAKE_CURRENT_SOURCE_DIR}/path/to/libssl.so)
#    endif()
#endif()

# 引入第三方头文件
include_directories("D:\\code\\c\\osg\\include")
# 引入第三方库文件
link_libraries("D:\\code\\c\\osg\\lib")

set(LIB_OSG_PATH "D:\\code\\c\\osg\\lib")

find_package(Qt5 COMPONENTS Widgets REQUIRED)

if(ANDROID)
  add_library(demoOSG SHARED
    main.cpp
    mainwindow.cpp
    mainwindow.h
    mainwindow.ui
    rotatecallback.cpp
    rotatecallback.h
    infocallback.cpp
    infocallback.h
  )
else()
  add_executable(demoOSG
    main.cpp
    mainwindow.cpp
    mainwindow.h
    mainwindow.ui
    rotatecallback.cpp
    rotatecallback.h
    infocallback.cpp
    infocallback.h
  )
endif()

target_link_libraries(demoOSG PRIVATE Qt5::Widgets
    ${LIB_OSG_PATH}\\osg.lib
    ${LIB_OSG_PATH}\\osgViewer.lib
    ${LIB_OSG_PATH}\\osgDB.lib
    ${LIB_OSG_PATH}\\OpenThreads.lib)
