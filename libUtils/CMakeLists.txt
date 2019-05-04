file(GLOB sources "*.cpp")
file(GLOB headers "*.hpp")

set(JSON_BuildTests OFF CACHE INTERNAL "")

add_library(Utils ${sources} ${headers})

add_subdirectory(nlohmann_json)
add_subdirectory(cxxopts)
