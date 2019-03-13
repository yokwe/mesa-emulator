#
# Makefile
#

LOG4CPP_CONFIG    := data/debug.properties
export LOG4CPP_CONFIG


.PHONY: all qt5-default clean distclean
.PHONY: run-app


all:
	(cd src/util;    make all)
#	(cd src/xns;     make all)
#	(cd src/courier; make all)
	(cd src/app;     make all)

qmake:
	(cd src/util;    qmake)
#	(cd src/xns;     qmake)
#	(cd src/courier; qmake)
	(cd src/app;     qmake)
	qmake --version

qt5-default:
	sudo apt-get install qt5-default

clean:
	echo "PATH = $(PATH)"
	rm -rf tmp/build
	mkdir  -p tmp/build
	mkdir  tmp/build/util
	mkdir  tmp/build/xns
	mkdir  tmp/build/courier
	mkdir  tmp/build/app

distclean: clean
	rm -f  src/*/Makefile
	rm -f  src/*/Makefile.Debug
	rm -f  src/*/Makefile.Release
	rm -f  src/*/object_script.*.Debug
	rm -f  src/*/object_script.*.Release


run-app: all
	echo -n >tmp/debug.log
	tmp/build/app/app

