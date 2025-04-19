cmake_minimum_required(VERSION 3.28)
project(21BenProjectSFML)

# Use C++14
set(CMAKE_CXX_STANDARD 17)

# Point CMake to your Homebrew SFML installation (adjust path if needed)
set(SFML_DIR "/opt/homebrew/opt/sfml@2/lib/cmake/SFML")
find_package(SFML 2.6.2 REQUIRED COMPONENTS graphics window system)

# Include your project headers
include_directories(${PROJECT_SOURCE_DIR}/21Ben)

# List all your source files
set(SOURCES
        21Ben/Card.cpp
        21Ben/Counter.cpp
        21Ben/Dealer.cpp
        21Ben/Deck.cpp
        21Ben/Game.cpp
        21Ben/Hand.cpp
        21Ben/main.cpp
        21Ben/MainMenu.cpp
        21Ben/OptimalPlay.cpp
        21Ben/Player.cpp
        21Ben/PlayMenu.cpp
        21Ben/SimPlayer.cpp
        21Ben/SimulateMenu.cpp
        21Ben/Simulation.cpp
        21Ben/StatisticsMenu.cpp
        21Ben/StatsTracker.cpp
        21Ben/Texture.cpp
)

# Define your executable target
add_executable(21BenProjectSFML ${SOURCES})

# Link SFML libraries
target_link_libraries(21BenProjectSFML
        PRIVATE sfml-graphics sfml-window sfml-system
)

add_custom_command(TARGET 21BenProjectSFML POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_CURRENT_SOURCE_DIR}/21Ben/assets
        ${CMAKE_CURRENT_BINARY_DIR}/assets
)