TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt

# This file is split into Three sections
# The first configures Qt and the source files for all platforms
# The second is the linux build
# The third the mac build
# (if your using windows you will need to add a fourth one!)
# first lets remove Qt core and gui not going to need it
QT       -= core gui
# set the variable for the Maya Location, this will be different depending
# upon the platform / version of maya used
TARGET = Bob
# for for mac we need a bundle so change the name
macx:TARGET=Bob.bundle
# here we add the source files (and headers if required)
SOURCES+
# these are defines required by Maya to re-define some C++
# stuff, we will add some more later to tell what platform
# we are on as well
DEFINES+=REQUIRE_IOSTREAM \
         _BOOL
# These are the maya libs we need to link to, this will change depending
# upon which maya framework we use, just add them to the end of
# this list as required and they will be added to the build
MAYALIBS=-lOpenMaya \
        -lFoundation

# tell maya we're building for Mac
macx:DEFINES+=OSMac_
macx:MAYALOCATION=/Applications/Autodesk/maya2018
macx:CONFIG -= app_bundle

macx:INCLUDEPATH+=$$MAYALOCATION/include
macx:INCLUDEPATH += /include/glm

# under mac we need to build a bundle, to do this use
# the -bundle flag but we also need to not use -dynamic lib so
# remove this
macx:LIBS +=-bundle
mac:LIBS -=-dynamiclib

macx:LIBS += -L$$MAYALOCATION/Maya.app/Contents/MacOS \
             $$MAYALIBS

HEADERS += \
    bobnode.h \
    brick.h \
    grid.h \
    voxelizer.h \
    voxelizer.h \
    graph.h

SOURCES += \
    bobnode.cpp \
    brick.cpp \
    grid.cpp \
    pluginmain.cpp \
    voxelizer.cpp \
    graph.cpp

QMAKE_CXXFLAGS += -Wsign-compare
