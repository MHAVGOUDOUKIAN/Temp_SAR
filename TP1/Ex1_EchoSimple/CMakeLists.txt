cmake_minimum_required(VERSION 3.0)
set(CMAKE_CXX_STANDARD 11)

set(PROJECT_NAME main)
project(${PROJECT_NAME})

message("Starting cmake process..")

# Very important, otherwise your executable will be considered as a shared library by your OS and no as an executable
set (CMAKE_CXX_FLAGS "-no-pie")

add_executable(client
    Client.cpp    
)

add_executable(server
    Serveur.cpp    
)

target_link_libraries(client
    pthread
)

target_link_libraries(server
    pthread
)

set(PERMISSIONS OWNER_READ 
    OWNER_WRITE 
    OWNER_EXECUTE 
    GROUP_READ 
    GROUP_WRITE 
    GROUP_EXECUTE 
    WORLD_READ 
    WORLD_WRITE 
    WORLD_EXECUTE
)