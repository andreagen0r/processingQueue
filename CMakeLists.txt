cmake_minimum_required(VERSION 3.16)

project(QueueProcessing VERSION 0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt6 6.5 REQUIRED COMPONENTS Quick Concurrent)

qt_standard_project_setup(REQUIRES 6.5)

qt_add_executable(appQueueProcessing
    main.cpp
)

qt_add_qml_module(appQueueProcessing
    URI QueueProcessing
    VERSION 1.0
    QML_FILES
        Main.qml
    SOURCES
        mymodel.h mymodel.cpp
        processingqueuemodel.h processingqueuemodel.cpp
        ModelData.h
)

# Qt for iOS sets MACOSX_BUNDLE_GUI_IDENTIFIER automatically since Qt 6.1.
# If you are developing for iOS or macOS you should consider setting an
# explicit, fixed bundle identifier manually though.
set_target_properties(appQueueProcessing PROPERTIES
#    MACOSX_BUNDLE_GUI_IDENTIFIER com.example.appQueueProcessing
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

target_link_libraries(appQueueProcessing
    PRIVATE
    Qt6::Quick
    Qt6::Concurrent
)

include(GNUInstallDirs)
install(TARGETS appQueueProcessing
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
