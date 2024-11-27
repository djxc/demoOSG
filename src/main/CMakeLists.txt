file(GLOB_RECURSE srcs CONFIGURE_DEPENDS src/*.cpp include/*.h)
add_library(main STATIC ${srcs})
target_include_directories(main PUBLIC include)
target_link_libraries(main PUBLIC callback)
target_link_libraries(main PUBLIC userScene)

