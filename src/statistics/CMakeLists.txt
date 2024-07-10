add_library(ktailctl_statistics STATIC statistics_model.cpp)
target_link_libraries(ktailctl_statistics PUBLIC Qt6::Core)
add_library(KTailctl::Statistics ALIAS ktailctl_statistics)
