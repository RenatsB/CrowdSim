TEMPLATE = lib
TARGET = CrowdLib

QT += core
CONFIG += console c++17
CONFIG -= app_bundle

DEFINES += QT_DEPRECATED_WARNINGS
INCLUDEPATH += $$PWD/include

HEADERS += $$files($$PWD/include/*.h)
SOURCES += $$files($$PWD/src/*.cpp)

OTHER_FILES += $$files(../README.md)
