# This specifies the exe name
TARGET=BlackFriday
# where to put the .o files
OBJECTS_DIR=obj
# core Qt Libs to use add more here if needed.
QT+=gui opengl core

# as I want to support 4.8 and 5 this will set a flag for some of the mac stuff
# mainly in the types.h file for the setMacVisual which is native in Qt5
isEqual(QT_MAJOR_VERSION, 5) {
        cache()
        DEFINES +=QT5BUILD
}
# where to put moc auto generated files
MOC_DIR=moc
# on a mac we don't create a .app bundle file ( for ease of multiplatform use)
CONFIG += console c++17
CONFIG-=app_bundle
# Auto include all .cpp files in the project src directory (can specifiy individually if required)
INCLUDEPATH += $$PWD/include \
               $$PWD/../crowdLib/include

HEADERS += $$files($$PWD/include/*.h)
SOURCES += $$files($$PWD/src/*.cpp)

OTHER_FILES += $$files(../README.md)
# where our exe is going to live (root of project)
DESTDIR=./
# add the glsl shader files
OTHER_FILES+= shaders/*.glsl
# were are going to default to a console app
# note each command you add needs a ; as it will be run as a single line
# first check if we are shadow building or not easiest way is to check out against current

NGLPATH=$$(NGLDIR)
isEmpty(NGLPATH){ # note brace must be here
        message("including $HOME/NGL")
        include($(HOME)/NGL/UseNGL.pri)
}
else{ # note brace must be here
        message("Using custom NGL location")
        include($(NGLDIR)/UseNGL.pri)
}

LIBS += -L../crowdLib -lCrowdLib

QMAKE_CXXFLAGS += -std=c++17
QMAKE_CXXFLAGS += -O0

QMAKE_RPATHDIR += $$PWD/../crowdLib
