######################################################################
# Automatically generated by qmake (3.0) Wed Oct 30 14:05:52 2013
######################################################################

TARGET   = courier

TEMPLATE = lib
CONFIG  += staticlib

# Input
HEADERS += Courier.h   Block.h   StreamOf.h   NIC.h   IDP.h   RIP.h   PEX.h   Time.h
SOURCES += Courier.cpp Block.cpp StreamOf.cpp NIC.cpp IDP.cpp RIP.cpp PEX.cpp Time.cpp

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
