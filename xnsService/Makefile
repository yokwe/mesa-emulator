#
# Makefile
#

LOG4CPP_CONFIG    := data/debug.properties
export LOG4CPP_CONFIG

.PHONY: all qt4-default qt5-default clean distclean
.PHONY: guam guam-headless test
.PHONY: run-guam run-guam-headless run-test
.PHONY: callgrind memecheck tar
.PHONY: qmake
.PHONY: qmake-raspi mount-raspi umount-raspi
.pHONY: qmake-win32 run-guam.exe run-guam-headless.exe run-test.exe


all:
	(cd src/core; make all)
	(cd src/util; make all)
	(cd src/app;  make all)

qt4-default:
	sudo apt-get install qt4-default

qt5-default:
	sudo apt-get install qt5-default

clean:
	echo "PATH = $(PATH)"
	rm -rf tmp/build
	mkdir  tmp/build
	mkdir  tmp/build/app
	mkdir  tmp/build/core
	mkdir  tmp/build/util

distclean: clean
	rm -f  src/*/Makefile
	rm -f  src/*/Makefile.Debug
	rm -f  src/*/Makefile.Release
	rm -f  src/*/object_script.*.Debug
	rm -f  src/*/object_script.*.Release


run-app: all
	echo -n >tmp/debug.log
	tmp/build/app/app

#
# BUILD FOR NATIVE
#
qmake:
	(cd src/app;  qmake)
	(cd src/core; qmake)
	(cd src/util; qmake)
	qmake --version
