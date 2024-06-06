enable_testing()
add_executable(test_data
    test_data.cpp
)
target_link_libraries(test_data
    Qt6::Test
    ktailctl_data
    ktailctl_common
)
add_test(NAME test_data COMMAND test_data)
