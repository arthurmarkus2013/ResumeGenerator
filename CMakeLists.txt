cmake_minimum_required(VERSION 3.5)

project(ResumeGenerator VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(XercesC REQUIRED)
find_package(unofficial-libharu CONFIG REQUIRED)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets Sql LinguistTools)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets Sql LinguistTools)

configure_file(TemplateSchema.xsd TemplateSchema.xsd NEWLINE_STYLE LF)

set(TS_FILES ResumeGenerator_en_US.ts)

set(PROJECT_SOURCES
        ItemTemplateInfo.h
        templateparser.h
        templateparser.cpp
        PersonalInfoEntry.h
        WorkExperienceEntry.h
        EducationEntry.h
        databaseoperations.h
        databaseoperations.cpp
        exporter.h
        exporter.cpp
        workexperiencedatamodel.h
        workexperiencedatamodel.cpp
        educationdatamodel.h
        educationdatamodel.cpp
        main.cpp
        mainwindow.cpp
        mainwindow.h
        mainwindow.ui
        ${TS_FILES}
)

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(ResumeGenerator
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}
    )
    qt_create_translation(QM_FILES ${CMAKE_SOURCE_DIR} ${TS_FILES})
endif()

target_link_libraries(ResumeGenerator PRIVATE Qt${QT_VERSION_MAJOR}::Widgets Qt${QT_VERSION_MAJOR}::Sql)

target_link_libraries(ResumeGenerator PRIVATE XercesC::XercesC unofficial::libharu::hpdf)

set_target_properties(ResumeGenerator PROPERTIES
    WIN32_EXECUTABLE TRUE
)

install(TARGETS ResumeGenerator
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR})

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(ResumeGenerator)
endif()
