add_library(ktailctl_data STATIC json.cpp location_data.cpp peer_data.cpp
                                 status_data.cpp)
target_link_libraries(ktailctl_data PUBLIC nlohmann_json::nlohmann_json
                                           Qt6::Core)
add_library(KTailctl::Data ALIAS ktailctl_data)
