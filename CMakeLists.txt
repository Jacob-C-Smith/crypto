# Need CMake 3.0
cmake_minimum_required (VERSION 3.0)

# The name of the repository
project ("crypto")

# Set the correct paths
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/build)
set(CMAKE_PREFIX_PATH ${CMAKE_SOURCE_DIR}/extern/)

# Comment out for Debug mode
set(IS_DEBUG_BUILD CMAKE_BUILD_TYPE STREQUAL "Debug")

# Set debug mode
if (${IS_DEBUG_BUILD})
    add_compile_definitions(NDEBUG)
endif()

# Add the crypto project
if ( NOT "${HAS_CRYPTO}")

    # Log
    message("[crypto] Adding crypto project")

    # Let any parent project know that it has the crypto project
    set(HAS_CRYPTO CACHE true PARENT_SCOPE)
endif()

# Add source to the executable
add_executable(crypto_example "crypto.c" "main.c")
target_include_directories(crypto_example PUBLIC include include/crypto/)

# Add source to the library
add_library(crypto SHARED "crypto.c")
target_include_directories(crypto PUBLIC include include/crypto/)