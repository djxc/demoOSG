file(GLOB_RECURSE srcs CONFIGURE_DEPENDS src/*.cpp include/*.h)
add_library(callback STATIC ${srcs})
target_include_directories(callback PUBLIC include)