set(
	sources
	main.cpp
)

add_executable(sif ${sources})
target_include_directories(sif PRIVATE "${CMAKE_SOURCE_DIR}")
target_link_libraries(sif PRIVATE Sif Utils nlohmann_json::nlohmann_json cxxopts)

include(GNUInstallDirs)
install(TARGETS sif DESTINATION "${CMAKE_INSTALL_BINDIR}")

