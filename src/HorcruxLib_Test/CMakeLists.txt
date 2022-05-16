cmake_minimum_required(VERSION 3.14)
project(HorcruxLib_Test VERSION 1.0)

set(CMAKE_CXX_STANDARD 17)

if (NOT googletest)

	message(STATUS "Faccio fetch content di google test!!")
    include(FetchContent)
    FetchContent_Declare(
      googletest
      # Specify the commit you depend on and update it regularly.
      URL https://github.com/google/googletest/archive/refs/heads/main.zip
    )
    # For Windows: Prevent overriding the parent project's compiler/linker settings
    set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
    FetchContent_MakeAvailable(googletest)
endif()

enable_testing()
# Now simply link against gtest or gtest_main as needed. Eg
add_executable(${PROJECT_NAME} HorcruxLib_Test.cpp)
target_link_libraries(${PROJECT_NAME} 
    Boost::program_options 
    Boost::iostreams
    OpenSSL::SSL
    HorcruxLib
    gtest_main
)
add_test(NAME HorcruxLib_Test COMMAND ${PROJECT_NAME})


set_property(TARGET ${PROJECT_NAME} PROPERTY VS_DEBUGGER_WORKING_DIRECTORY ${CMAKE_INSTALL_PREFIX})

message(STATUS "Dopo la fase di build installo l'eseguibile ${PROJECT_NAME}.exe nella cartella ${CMAKE_INSTALL_PREFIX}")
install(TARGETS "${PROJECT_NAME}" DESTINATION ${CMAKE_INSTALL_PREFIX}) 

