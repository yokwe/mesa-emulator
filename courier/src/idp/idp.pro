######################################################################
# Automatically generated by qmake (3.0) Wed Oct 30 14:05:52 2013
######################################################################

TARGET   = idp

TEMPLATE = lib
CONFIG  += staticlib

# Input
HEADERS += NIC.h   Packet.h
SOURCES += NIC.cpp Packet.cpp

###############################################

INCLUDEPATH += .

QMAKE_CXXFLAGS += -std=c++14 -Wall -Werror -g

contains(QT_MAJOR_VERSION, 4) {
        QMAKE_CXXFLAGS += -Wno-unused-local-typedefs
}

DESTDIR     = ../../tmp/build/$$TARGET
OBJECTS_DIR = ../../tmp/build/$$TARGET
MOC_DIR     = ../../tmp/build/$$TARGET
RCC_DIR     = ../../tmp/build/$$TARGET
UI_DIR      = ../../tmp/build/$$TARGET
