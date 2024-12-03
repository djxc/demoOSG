file(GLOB_RECURSE srcs CONFIGURE_DEPENDS src/*.cpp include/*.h)
add_library(precipitations STATIC ${srcs})
target_include_directories(precipitations PUBLIC include)

