######################################################################
# Automatically generated by qmake (3.0) Wed Oct 30 13:34:52 2013
######################################################################

TARGET   = simple-opcode

TEMPLATE = lib
CONFIG  += staticlib

# Input

HEADERS += Interpreter.h   Opcode.h
SOURCES += Interpreter.cpp Opcode.cpp

SOURCES += Opcode_bitblt.cpp Opcode_block.cpp Opcode_control.cpp Opcode_process.cpp Opcode_special.cpp
SOURCES += OpcodeMop0xx.cpp OpcodeMop1xx.cpp OpcodeMop2xx.cpp OpcodeMop3xx.cpp
SOURCES += OpcodeEsc.cpp

###############################################

INCLUDEPATH += .

QMAKE_CXXFLAGS += -std=c++14 -Wall -Werror -g

contains(QT_MAJOR_VERSION, 4) {
        QMAKE_CXXFLAGS += -fno-strict-overflow
        QMAKE_CXXFLAGS += -Wno-unused-local-typedefs
}

DESTDIR     = ../../tmp/build/$$TARGET
OBJECTS_DIR = ../../tmp/build/$$TARGET
MOC_DIR     = ../../tmp/build/$$TARGET
RCC_DIR     = ../../tmp/build/$$TARGET
UI_DIR      = ../../tmp/build/$$TARGET
