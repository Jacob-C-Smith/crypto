# Need CMake 3.28.0
cmake_minimum_required (VERSION 3.28.0)

# The name of the repository
project ("crypto")

# Set the correct paths
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/build)
set(CMAKE_PREFIX_PATH ${CMAKE_SOURCE_DIR}/extern/)

# Set compiler warnings
if(MSVC)
    # TODO
else()

    # For clang
    #set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -lrt -std=gnu99 -Wall -Wextra -Wpedantic -Weverything -Wno-declaration-after-statement -Ofast")

    # For GCC
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -std=c23 -Wall -Wextra -O3 -pthread -lpthread")
    add_compile_definitions(__USE_POSIX199309)
    add_compile_options(-Wall -Wextra -Wpointer-arith -Wstrict-prototypes -Wformat-security -Wfloat-equal -Wshadow -Wconversion -pthread -lpthread -Wlogical-not-parentheses -Wnull-dereference)
endif()

# Comment out for Debug mode
set(IS_DEBUG_BUILD CMAKE_BUILD_TYPE STREQUAL "Debug")

# Build crypto with semaphore
add_compile_definitions(BUILD_CRYPTO_WITH_SEMAPHORE)

# Build crypto with monitor
add_compile_definitions(BUILD_CRYPTO_WITH_MONITOR)

# Set debug mode
if (${IS_DEBUG_BUILD})
    add_compile_definitions(NDEBUG)
endif()

# Find the log module
if ( NOT "${HAS_LOG}")

    # Add the log directory
    add_subdirectory(${CMAKE_SOURCE_DIR}/extern/log ${CMAKE_SOURCE_DIR}/build/log)

    # Let any parent project know that this project has log
    if(NOT PROJECT_IS_TOP_LEVEL)
        set(HAS_LOG true PARENT_SCOPE)
        set(LOG_INCLUDE_DIR ${LOG_INCLUDE_DIR} PARENT_SCOPE)
    endif()
endif()

# Add the crypto project
if ( NOT "${HAS_CRYPTO}")

    # Log
    message("[crypto] Adding crypto project")
    
    set(CRYPTO_INCLUDE_DIR ${CMAKE_CURRENT_LIST_DIR}/include)

    # Let any parent project know that it has the crypto project
    if(NOT PROJECT_IS_TOP_LEVEL)
        set(HAS_CRYPTO true PARENT_SCOPE)
        set(CRYPTO_INCLUDE_DIR ${CRYPTO_INCLUDE_DIR} PARENT_SCOPE)
    endif()
endif()

# Add source to the executable
add_executable(crypto_example "main.c")
add_dependencies(crypto_example crypto log)
target_include_directories(crypto_example PUBLIC ${CRYPTO_INCLUDE_DIR} ${LOG_INCLUDE_DIR})
target_link_libraries(crypto_example crypto log pthread)

# Add source to the library
add_library(crypto SHARED "crypto.c" "rsa.c")
add_dependencies(crypto log)
target_include_directories(crypto PUBLIC ${CRYPTO_INCLUDE_DIR} ${LOG_INCLUDE_DIR})
target_link_libraries(crypto log pthread)
