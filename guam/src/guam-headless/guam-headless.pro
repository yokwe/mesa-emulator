######################################################################
# Automatically generated by qmake (3.0) Wed Oct 30 15:10:08 2013
######################################################################

TARGET = guam-headless

TEMPLATE = app

unix {
	QMAKE_POST_LINK = sudo setcap CAP_NET_RAW+pe $(TARGET)
}

QMAKE_LFLAGS   += -g

unix {
	QMAKE_LFLAGS   += -rdynamic
}

win32 {
	QMAKE_LFLAGS   += -static
}

contains(QT_MAJOR_VERSION, 4) {
        QMAKE_CXXFLAGS += -Wno-unused-local-typedefs
}

LIBS += ../../tmp/build/mesa/libmesa.a
LIBS += ../../tmp/build/opcode/libopcode.a
LIBS += ../../tmp/build/agent/libagent.a
LIBS += ../../tmp/build/util/libutil.a

LIBS += -llog4cpp

POST_TARGETDEPS += ../../tmp/build/mesa/libmesa.a
POST_TARGETDEPS += ../../tmp/build/opcode/libopcode.a
POST_TARGETDEPS += ../../tmp/build/agent/libagent.a
POST_TARGETDEPS += ../../tmp/build/util/libutil.a

# Input
SOURCES += LoadGerm.cpp

###############################################

INCLUDEPATH += .

QMAKE_CXXFLAGS += -std=c++0x -Wall -Werror -g

DESTDIR     = ../../tmp/build/$$TARGET
OBJECTS_DIR = ../../tmp/build/$$TARGET
MOC_DIR     = ../../tmp/build/$$TARGET
RCC_DIR     = ../../tmp/build/$$TARGET
UI_DIR      = ../../tmp/build/$$TARGET
