add_ktailctl_test(test_util_format_duration_human_readable
    SOURCES
        test_format_duration_human_readable.cpp
        ${CMAKE_SOURCE_DIR}/src/util.cpp
    LIBS KF6::GuiAddons)
