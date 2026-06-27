target_sources(ktailctl PRIVATE pinger.hpp pinger.cpp)
target_include_directories(ktailctl PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(ktailctl PRIVATE KF6::ThreadWeaver)
