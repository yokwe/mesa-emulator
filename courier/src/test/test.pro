######################################################################
# Automatically generated by qmake (3.0) Wed Oct 30 14:24:50 2013
######################################################################

TARGET   = test
TEMPLATE = app

# Input
HEADERS += testBase.h
SOURCES += testBase.cpp

SOURCES += testMain.cpp testCourier.cpp

LIBS += ../../tmp/build/courier/libcourier.a
LIBS += ../../tmp/build/util/libutil.a

LIBS += -lcppunit -llog4cpp

POST_TARGETDEPS += ../../tmp/build/courier/libcourier.a
POST_TARGETDEPS += ../../tmp/build/util/libutil.a

###############################################

INCLUDEPATH += .

QMAKE_CXXFLAGS += -std=c++14 -Wall -Werror -g

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