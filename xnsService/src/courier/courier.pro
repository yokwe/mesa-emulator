######################################################################
# Automatically generated by qmake (3.0) Wed Oct 30 14:05:52 2013
######################################################################

TARGET   = courier

TEMPLATE = lib
CONFIG  += staticlib

# Input
HEADERS += ByteBuffer.h   Courier.h   Network.h   SocketManager.h
SOURCES += ByteBuffer.cpp Courier.cpp Network.cpp SocketManager.cpp

HEADERS += socket/SocketEcho.h   socket/SocketPEX.h   socket/SocketRouting.h
SOURCES += socket/SocketEcho.cpp socket/SocketPEX.cpp socket/SocketRouting.cpp

HEADERS += stub/StubDatagram.h   stub/StubEcho.h   stub/StubError.h   stub/StubEthernet.h   stub/StubRouting.h
SOURCES += stub/StubDatagram.cpp stub/StubEcho.cpp stub/StubError.cpp stub/StubEthernet.cpp stub/StubRouting.cpp

HEADERS += stub/StubExpeditedCourier.h   stub/StubPacketExchange.h   stub/StubProtocol.h   stub/StubTime2.h
SOURCES += stub/StubExpeditedCourier.cpp stub/StubPacketExchange.cpp stub/StubProtocol.cpp stub/StubTime2.cpp

###############################################

INCLUDEPATH += .

QMAKE_CXXFLAGS += -std=c++0x -Wall -Werror -g

contains(QT_MAJOR_VERSION, 4) {
        QMAKE_CXXFLAGS += -Wno-unused-local-typedefs
}

DESTDIR     = ../../tmp/build/$$TARGET
OBJECTS_DIR = ../../tmp/build/$$TARGET
MOC_DIR     = ../../tmp/build/$$TARGET
RCC_DIR     = ../../tmp/build/$$TARGET
UI_DIR      = ../../tmp/build/$$TARGET
