cmake_minimum_required(VERSION 3.22)

#
# This file is generated only once,
# and is not re-generated if converter is called multiple times.
#
# User is free to modify the file as much as necessary
#

# Setup compiler settings
set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS ON)


# Define the build type
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE "Debug")
endif()

# Set the project name
set(CMAKE_PROJECT_NAME NB_games)

# Enable compile command to ease indexing with e.g. clangd
set(CMAKE_EXPORT_COMPILE_COMMANDS TRUE)

# Core project settings
project(${CMAKE_PROJECT_NAME})
message("Build type: " ${CMAKE_BUILD_TYPE})

# Enable CMake support for ASM and C languages
enable_language(C ASM)

# Create an executable object type
add_executable(${CMAKE_PROJECT_NAME}


    Library/game_mission_planer.cpp
    Library/game_init_task.cpp
    Library/heartbeat_task.cpp


    Library/component/game/game_task.cpp
    Library/component/game/menu.cpp
    Library/component/game/fsm/menu_state_main.cpp
    Library/component/game/fsm/menu_state_music.cpp
    Library/component/game/fsm/menu_state_standby.cpp
    Library/component/game/fsm/menu_state_playing_music.cpp


    Library/component/buzzer/buzzer_task.cpp
    Library/component/buzzer/buzzer.cpp

    Library/component/control/vrc.cpp


    Library/component/oled/oled.cpp
    Library/component/oled/oled_task.cpp

    Library/Core/Blackboard/blackboard.cpp

)



# Add STM32CubeMX generated sources
add_subdirectory(cmake/stm32cubemx)

# Link directories setup
target_link_directories(${CMAKE_PROJECT_NAME} PRIVATE
    # Add user defined library search paths
    
    ${CMAKE_CURRENT_SOURCE_DIR}
)

# Add sources to executable
target_sources(${CMAKE_PROJECT_NAME} PRIVATE
    # Add user sources here
)

# Add include paths
target_include_directories(${CMAKE_PROJECT_NAME} PRIVATE
    # Add user defined include paths
    ${CMAKE_CURRENT_SOURCE_DIR}/Library
    ${CMAKE_CURRENT_SOURCE_DIR}/Library/Core/FSM
    ${CMAKE_CURRENT_SOURCE_DIR}/Library/Component/buzzer
    ${CMAKE_CURRENT_SOURCE_DIR}/Library/Component/buzzer/songs
    ${CMAKE_CURRENT_SOURCE_DIR}/Library/Component/oled
    ${CMAKE_CURRENT_SOURCE_DIR}/Library/Component/game
    ${CMAKE_CURRENT_SOURCE_DIR}/Library/Component/control
    ${CMAKE_CURRENT_SOURCE_DIR}/Library/Core/Blackboard
)

# Add project symbols (macros)
target_compile_definitions(${CMAKE_PROJECT_NAME} PRIVATE
    # Add user defined symbols
)

# Remove wrong libob.a library dependency when using cpp files
list(REMOVE_ITEM CMAKE_C_IMPLICIT_LINK_LIBRARIES ob)

# Add linked libraries
target_link_libraries(${CMAKE_PROJECT_NAME}
    stm32cubemx

    # Add user defined libraries
)
