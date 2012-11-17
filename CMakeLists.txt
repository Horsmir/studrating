project(studrating)
cmake_minimum_required(VERSION 2.6)

set(APPLICATION_NAME "StudRating")
set(APPLICATION_FILE_NAME "studrating")

if (NOT CMAKE_BUILD_TYPE)
    set ( CMAKE_BUILD_TYPE Release )
endif (NOT CMAKE_BUILD_TYPE)

if (CMAKE_BUILD_TYPE MATCHES [Dd]ebug)
    message(STATUS "Debug build")
    add_definitions(-DDEBUG)
else()
    message(STATUS "non-debug build")
    add_definitions(-DNDEBUG)
    add_definitions(-DQT_NO_DEBUG_OUTPUT)
endif()

if (NOT CMAKE_INSTALL_PREFIX)
	set(CMAKE_INSTALL_PREFIX /usr)
endif(NOT CMAKE_INSTALL_PREFIX)
message(STATUS "CMAKE_INSTALL_PREFIX = ${CMAKE_INSTALL_PREFIX}")

set(MAJOR_VERSION 0)
set(MINOR_VERSION 1)
set(PATCH_VERSION 1)
set(STUDRATING_VERSION ${MAJOR_VERSION}.${MINOR_VERSION}.${PATCH_VERSION})
add_definitions(-DSTUDRATING_VERSION=\"${STUDRATING_VERSION}\")
message(STATUS "")
message(STATUS "${APPLICATION_NAME} version: ${STUDRATING_VERSION}")
message(STATUS "")

find_package(Qt4 REQUIRED)

include_directories(${QT_INCLUDES} ${CMAKE_CURRENT_BINARY_DIR} ${PROJECT_SOURCE_DIR}/datab ${PROJECT_SOURCE_DIR}/help-browser)

configure_file("${PROJECT_SOURCE_DIR}/globals.h.in" "${PROJECT_BINARY_DIR}/globals.h")

set(helpbrowser_SRCS help-browser/helpbrowser.cpp )
set(helpbrowser_UIS help-browser/helpform.ui)

set(datab_SRCS datab/htmlgenerator.cpp datab/criterion.cpp datab/databasestudrating.cpp datab/group.cpp datab/student.cpp)

set(models_SRCS models/ratingmodel.cpp models/criteriamodel.cpp models/groupmodel.cpp)

set(studrating_SRCS ${models_SRCS} ${datab_SRCS} ${helpbrowser_SRCS} adddatedialog.cpp studentlistdialog.cpp createdbdialog.cpp studratingmanager.cpp studrating.cpp main.cpp)
set(studrating_UIS mainwindow.ui createdbdlg.ui studentlistdlg.ui adddatedlg.ui ${helpbrowser_UIS})
set(studrating_QRCS studrating.qrc)

qt4_automoc(${studrating_SRCS})
QT4_WRAP_UI(UI_HEADERS ${studrating_UIS})
QT4_ADD_RESOURCES(QRC_SOURCES ${studrating_QRCS})

add_executable(studrating ${studrating_SRCS} ${UI_HEADERS} ${QRC_SOURCES})
target_link_libraries(studrating ${QT_QTCORE_LIBRARY} ${QT_QTGUI_LIBRARY})

install(TARGETS studrating RUNTIME DESTINATION ${CMAKE_INSTALL_PREFIX}/bin)
install(DIRECTORY doc/ DESTINATION ${CMAKE_INSTALL_PREFIX}/share/doc/${APPLICATION_FILE_NAME}/html PATTERN "*~" EXCLUDE)
