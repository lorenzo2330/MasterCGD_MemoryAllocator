#Requisiti minimi di ambiente
cmake_minimum_required(VERSION 3.14)
project(MemoryManager VERSION 0.1.0 LANGUAGES CXX)

#Requisiti minimi di compilazione
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

#Flag per i warning, dipendenti dal compilatore
if(MSVC)
    add_compile_options(/W4 /WX)
else()
    add_compile_options(-Wall -Wextra -Wpedantic)
endif()

#Parametri configurabili a compile time (es da command line cmake -DMM_GLOBAL_OVERRIDE=ON .., oppure da gui)
option(MM_GLOBAL_OVERRIDE "Replace global new/delete with MM" OFF)
option(MM_THREAD_SAFE_OPT "Enable mutex guards in allocators" ON)
option(MM_ENABLE_TESTS "Build test executables" ON)

# Target della libreria, con i relativi file sorgente e header
add_library(MemoryManagerLib STATIC
    # Core infrastructure
    src/core/AllocatorStats.cpp
    src/core/LeakTracker.cpp

    # Step 2 – SmallObjectAllocator (add when ready)
    # src/smallAllocator/Chunk.cpp
    # src/smallAllocator/FixedAllocator.cpp
    # src/smallAllocator/SmallObjectAllocator.cpp

    # Step 3 – GeneralPurposeAllocator (add when ready)
    # src/generalAllocator/GeneralPurposeAllocator.cpp

    # Step 4 – MemoryManager hub (add when ready)
    # src/hub/MemoryManager.cpp
)

#Path per i vari include
target_include_directories(MemoryManagerLib
    PUBLIC  ${CMAKE_CURRENT_SOURCE_DIR}/include
    PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src
)

#Imposta le varie macro per abilitare le funzionalità a compile time, in base alle opzioni scelte
if(MM_GLOBAL_OVERRIDE)
    target_compile_definitions(MemoryManagerLib PUBLIC USE_MM_GLOBAL)
endif()

if(MM_THREAD_SAFE_OPT)
    target_compile_definitions(MemoryManagerLib PUBLIC MM_THREAD_SAFE=1)
endif()

if(MM_THREAD_SAFE_OPT AND UNIX)
    find_package(Threads REQUIRED)
    target_link_libraries(MemoryManagerLib PUBLIC Threads::Threads)
endif()

#Compilazione di main.cpp, produce il binario MemoryManagerApp
add_executable(MemoryManagerApp app/main.cpp)
target_link_libraries(MemoryManagerApp PRIVATE MemoryManagerLib)

#Abilita i test se richiesto
if(MM_ENABLE_TESTS)
    enable_testing()

    add_executable(RunTests
        tests/test_runner.cpp   # created in Step 2
    )
    target_link_libraries(RunTests PRIVATE MemoryManagerLib)
    target_include_directories(RunTests PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/tests
    )

    add_test(NAME AllTests COMMAND RunTests)
endif()