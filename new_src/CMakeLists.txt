add_subdirectory(Networks)
add_subdirectory(Nodes)
add_subdirectory(LocalClients)
add_subdirectory(Go)

add_executable(ktailctl2 main.cpp)
target_link_libraries(ktailctl2 PRIVATE Qt6::Widgets ktailctl_nodes)
target_include_directories(ktailctl2 PRIVATE ${PROJECT_SOURCE_DIR}/include)
