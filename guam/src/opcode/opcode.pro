######################################################################
# Automatically generated by qmake (3.0) Wed Oct 30 13:34:52 2013
######################################################################

TARGET   = opcode

TEMPLATE = lib
CONFIG  += staticlib

# Input

HEADERS += Interpreter.h   Opcode.h   Run.h
SOURCES += Interpreter.cpp Opcode.cpp Run.cpp

SOURCES += Opcode_bitblt.cpp Opcode_block.cpp Opcode_control.cpp Opcode_process.cpp
SOURCES += Opcode_simple.cpp Opcode_special.cpp

###############################################

INCLUDEPATH += .

QMAKE_CXXFLAGS += -std=c++0x -Wall -Werror -g

contains(QT_MAJOR_VERSION, 4) {
        QMAKE_CXXFLAGS += -fno-strict-overflow
        QMAKE_CXXFLAGS += -Wno-unused-local-typedefs
}

DESTDIR     = ../../tmp/build/$$TARGET
OBJECTS_DIR = ../../tmp/build/$$TARGET
MOC_DIR     = ../../tmp/build/$$TARGET
RCC_DIR     = ../../tmp/build/$$TARGET
UI_DIR      = ../../tmp/build/$$TARGET
