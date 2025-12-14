cmake_minimum_required(VERSION 3.10)

project(portspoof VERSION 1.3 LANGUAGES CXX)

# Set C++ Standard
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Find Threads (pthreads)
set(CMAKE_THREAD_PREFER_PTHREAD TRUE)
find_package(Threads REQUIRED)

# Add source directory
add_subdirectory(src)

# Install configuration files
include(GNUInstallDirs)
install(FILES tools/portspoof.conf tools/portspoof_signatures
        DESTINATION ${CMAKE_INSTALL_SYSCONFDIR})

