cmake_minimum_required(VERSION 3.14)
project(horcrux VERSION 1.0)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

set(Boost_USE_STATIC_LIBS   ON)

set(BOOST_TOOLSET msvc-14.1)

message(STATUS "cerco Boost ,almeno la versione 1.66.0 richiesta")
find_package(Boost 1.67.0 REQUIRED COMPONENTS
             program_options iostreams)

if (Boost_FOUND)
  message(STATUS "BOOST FOUND")
  include_directories (SYSTEM ${Boost_INCLUDE_DIR})

endif ()

message(STATUS "cerco OPENSSL")
find_package(OpenSSL REQUIRED)
if (OpenSSL_FOUND)
  include_directories (SYSTEM ${OPENSSL_INCLUDE_DIR})
  list(APPEND LIB_LIST ${OPENSSL_LIBRARIES})
  message(STATUS "OPENSSL FOUND INCLUDE_DIR: ${OPENSSL_INCLUDE_DIR}")
endif ()

add_subdirectory(HorcruxLib)
add_subdirectory(HorcruxLib_Test)


add_executable(${PROJECT_NAME} horcrux.cpp)

target_link_libraries(${PROJECT_NAME} Boost::program_options Boost::iostreams OpenSSL::SSL HorcruxLib )
set_property(TARGET ${PROJECT_NAME} PROPERTY VS_DEBUGGER_WORKING_DIRECTORY ${CMAKE_INSTALL_PREFIX})


message(STATUS "Dopo la compila installerò l'eseguibile ${PROJECT_NAME}.exe nella cartella ${CMAKE_INSTALL_PREFIX}")
install(TARGETS "${PROJECT_NAME}" DESTINATION ${CMAKE_INSTALL_PREFIX})

message(STATUS "installo le dll presenti nella cartella ${CMAKE_CURRENT_SOURCE_DIR}/dll/ nella cartella ${CMAKE_INSTALL_PREFIX}")
file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/dll/ DESTINATION ${CMAKE_INSTALL_PREFIX} FILES_MATCHING PATTERN "*.dll" )


