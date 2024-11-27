file(GLOB_RECURSE srcs CONFIGURE_DEPENDS src/*.cpp include/*.h)
add_library(userScene STATIC ${srcs})
target_include_directories(userScene PUBLIC include)

