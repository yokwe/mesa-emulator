######################################################################
# Automatically generated by qmake (3.0) Wed Oct 30 14:24:50 2013
######################################################################

TARGET   = showType
TEMPLATE = app

# Input
#HEADERS += 
SOURCES += main.cpp

#HEADERS += 
#SOURCES += 

LIBS += ../../tmp/build/symbols/libsymbols.a
LIBS += ../../tmp/build/mesa/libmesa.a
LIBS += ../../tmp/build/util/libutil.a

LIBS += -llog4cpp

POST_TARGETDEPS += ../../tmp/build/symbols/libsymbols.a
POST_TARGETDEPS += ../../tmp/build/mesa/libmesa.a
POST_TARGETDEPS += ../../tmp/build/util/libutil.a

###############################################

INCLUDEPATH += .

QMAKE_CXXFLAGS += -std=c++0x -Wall -Werror -g

win32 {
	QMAKE_LFLAGS   += -static
}

contains(QT_MAJOR_VERSION, 4) {
        QMAKE_CXXFLAGS += -Wno-unused-local-typedefs
}

DESTDIR     = ../../tmp/build/$$TARGET
OBJECTS_DIR = ../../tmp/build/$$TARGET
MOC_DIR     = ../../tmp/build/$$TARGET
RCC_DIR     = ../../tmp/build/$$TARGET
UI_DIR      = ../../tmp/build/$$TARGET
