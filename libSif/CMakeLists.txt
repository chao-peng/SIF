file(GLOB sources "*.cpp")
file(GLOB headers "*.hpp")

add_library(Sif ${sources} ${headers})
target_include_directories(Sif PRIVATE "${CMAKE_SOURCE_DIR}")
target_link_libraries(Sif PRIVATE Utils nlohmann_json::nlohmann_json)
