cmake_minimum_required(VERSION 3.16)

project(Mundial LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(Mundial main.cpp
    globales.h globales.cpp
    Lista.h
    jugador.h jugador.cpp
    equipo.h equipo.cpp
    partido.h partido.cpp
    mundial.h mundial.cpp
)

include(GNUInstallDirs)
install(TARGETS Mundial
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
