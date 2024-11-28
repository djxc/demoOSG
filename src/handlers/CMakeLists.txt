file(GLOB_RECURSE srcs CONFIGURE_DEPENDS src/*.cpp include/*.h)
add_library(handlers STATIC ${srcs})
target_include_directories(handlers PUBLIC include)

