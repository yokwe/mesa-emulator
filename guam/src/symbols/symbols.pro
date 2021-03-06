######################################################################
# Automatically generated by qmake (3.0) Wed Oct 30 13:34:52 2013
######################################################################

TARGET   = symbols

TEMPLATE = lib
CONFIG  += staticlib

# Input
HEADERS += BCD.h   BCDFile.h   BTIndex.h   CTXIndex.h   ExtIndex.h   HTIndex.h   LTIndex.h   MDIndex.h   SEIndex.h   Symbols.h   Tree.h
SOURCES += BCD.cpp BCDFile.cpp BTIndex.cpp CTXIndex.cpp ExtIndex.cpp HTIndex.cpp LTIndex.cpp MDIndex.cpp SEIndex.cpp Symbols.cpp Tree.cpp

HEADERS += DumpSymbol.h   ShowType.h
SOURCES += DumpSymbol.cpp ShowType.cpp

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
