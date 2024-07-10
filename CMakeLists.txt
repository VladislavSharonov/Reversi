cmake_minimum_required(VERSION 3.27)
project(Reversi VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Build
find_package(Qt6 COMPONENTS Core Qml Svg QuickControls2 REQUIRED)
qt_standard_project_setup()
qt_policy(SET QTP0001 NEW)

function(paths_to_relative FILE_PATHS RELATIVE_PATHS)
    foreach (FILE_PATH ${${FILE_PATHS}})
        file(RELATIVE_PATH RELATIVE_FILE_PATH ${CMAKE_CURRENT_SOURCE_DIR} ${FILE_PATH})
        list(APPEND ${RELATIVE_PATHS} ${RELATIVE_FILE_PATH})
    endforeach ()
    set(${RELATIVE_PATHS} ${${RELATIVE_PATHS}} PARENT_SCOPE)
endfunction()

# Resources
set(Reversi_RESOURCES "resources/resources.qrc")

file(GLOB_RECURSE Reversi_SOURCES
    "source/*.c"
    "source/*.cpp"
    "source/*.h"
    "source/*.hpp"
)

file(GLOB_RECURSE Reversi_UI_GLOBAL_PATHS "resources/ui/*.qml" "resources/ui/*.js")
paths_to_relative(Reversi_UI_GLOBAL_PATHS Reversi_UI)

qt_add_executable(${PROJECT_NAME} ${Reversi_SOURCES})
qt_add_qml_module(${PROJECT_NAME}
    URI "/"
    VERSION 1.0
    QML_FILES
        ${Reversi_UI}
    RESOURCES
        ${Reversi_RESOURCES}
    SOURCES
        ${Reversi_SOURCES}
)

target_include_directories(${PROJECT_NAME} PUBLIC ${Qt6_INCLUDE_DIRS})
target_link_libraries(${PROJECT_NAME} PUBLIC Qt6::Core Qt6::Qml Qt6::Svg Qt6::QuickControls2)

set_target_properties(${PROJECT_NAME} PROPERTIES
        #    MACOSX_BUNDLE_GUI_IDENTIFIER com.ReversiApplication
        MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
        MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
        MACOSX_BUNDLE TRUE
        WIN32_EXECUTABLE TRUE
)

include(GNUInstallDirs)
install(TARGETS ${PROJECT_NAME}
        BUNDLE DESTINATION .
        LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
        RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
