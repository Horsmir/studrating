project(studrating)
cmake_minimum_required(VERSION 2.6)
find_package(Qt4 REQUIRED)

include_directories(${QT_INCLUDES} ${CMAKE_CURRENT_BINARY_DIR})

set(studrating_SRCS studrating.cpp main.cpp)
qt4_automoc(${studrating_SRCS})
add_executable(studrating ${studrating_SRCS})
target_link_libraries(studrating ${QT_QTCORE_LIBRARY} ${QT_QTGUI_LIBRARY})
install(TARGETS studrating RUNTIME DESTINATION bin)
