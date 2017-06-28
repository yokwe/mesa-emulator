#
# Makefile
#

LOG4CPP_CONFIG    := data/debug.properties
export LOG4CPP_CONFIG

MXE_ROOT          := /opt/mxe
MXE_QMAKE         := $(MXE_ROOT)/usr/i686-pc-mingw32.static/qt/bin/qmake
PATH              := $(MXE_ROOT)/usr/bin:$(PATH)
export PATH

YYMMDDHHMMSS      := $(shell date '+%y%m%d-%H%M%S')
CALLGRIND_LOGFILE := tmp/callgrind/callgrind.$(YYMMDDHHMMSS)
MEMCHECK_LOGFILE  := tmp/memcheck/memcheck.$(YYMMDDHHMMSS)

RASPI_ROOT        := /mnt/raspi
RASPI_DEPLOY_ROOT := $(RASPI_ROOT)/home/pi/gaum

.PHONY: all qt4-default qt5-default clean distclean
.PHONY: guam guam-headless test floppy
.PHONY: run-guam run-guam-headless run-test
.PHONY: callgrind memecheck tar
.PHONY: qmake
.PHONY: qmake-raspi mount-raspi umount-raspi
.pHONY: qmake-win32 run-guam.exe run-guam-headless.exe run-test.exe


all:
	(cd src/mesa;          make all)
	(cd src/simple-opcode; make all)
	(cd src/agent;         make all)
	(cd src/trace;         make all)
	(cd src/util;          make all)
	(cd src/test;          make all)
	(cd src/guam-headless; make all)
	(cd src/guam;          make all)
	(cd src/floppy;        make all)

qt4-default:
	sudo apt-get install qt4-default

qt5-default:
	sudo apt-get install qt5-default

clean:
	echo "PATH = $(PATH)"
	rm -rf tmp/build
	mkdir  -p tmp/build
	mkdir  tmp/build/mesa
	mkdir  tmp/build/simple-opcode
	mkdir  tmp/build/agent
	mkdir  tmp/build/trace
	mkdir  tmp/build/util
	mkdir  tmp/build/test
	mkdir  tmp/build/guam-headless
	mkdir  tmp/build/guam
	mkdir  tmp/build/floppy

distclean: clean
	rm -f  src/*/Makefile
	rm -f  src/*/Makefile.Debug
	rm -f  src/*/Makefile.Release
	rm -f  src/*/object_script.*.Debug
	rm -f  src/*/object_script.*.Release

guam:
	(cd src/mesa;          make all)
	(cd src/simple-opcode; make all)
	(cd src/agent;         make all)
	(cd src/util;          make all)
	(cd src/guam;          make all)

guam-headless:
	(cd src/mesa;          make all)
	(cd src/simple-opcode; make all)
	(cd src/agent;         make all)
	(cd src/util;          make all)
	(cd src/guam-headless; make all)

test:
	(cd src/mesa;          make all)
	(cd src/simple-opcode; make all)
	(cd src/agent;         make all)
	(cd src/util;          make all)
	(cd src/test;          make all)

floppy:
	(cd src/util;          make all)
	(cd src/agent;         make all)
	(cd src/floppy;        make all)

trace:
	(cd src/mesa;          make all)
	(cd src/simple-opcode; make all)
	(cd src/agent;         make all)
	(cd src/util;          make all)
	(cd src/trace;         make all)

run-trace: trace
	echo -n >tmp/debug.log
	tmp/build/trace/trace

run-guam: guam
	echo -n >tmp/debug.log
	tmp/build/guam/guam

run-guam-headless: guam-headless
	echo -n >tmp/debug.log
	tmp/build/guam-headless/guam-headless

run-test: test
	echo -n >tmp/debug.log
	tmp/build/test/test

run-floppy: floppy
	echo -n >tmp/debug.log
	tmp/build/floppy/floppy

create-floppy-image:
	mkdir -p tmp/floppy/image
	rm -f tmp/floppy/image/floppy144
	dd if=/dev/zero of=tmp/floppy/image/floppy144 bs=512 count=2880

callgrind:
	mkdir -p tmp/callgrind
	echo -n >tmp/debug.log
	echo -n >$(CALLGRIND_LOGFILE)
	sudo -E valgrind --tool=callgrind --callgrind-out-file=$(CALLGRIND_LOGFILE) --dump-instr=yes tmp/build/guam-headless/guam-headless

memcheck:
	mkdir -p tmp/memcheck
	echo -n >tmp/debug.log
	echo -n >$(MEMCHECK_LOGFILE)
	sudo -E valgrind --tool=memcheck \
	  --read-var-info=yes --track-origins=yes --leak-check=full \
	  tmp/build/guam-headless/guam-headless 2>&1 | tee $(MEMCHECK_LOGFILE)

tar:
	rm -rf tmp/guam
	mkdir tmp/guam
	cp -r Makefile tmp/guam
	mkdir tmp/guam/data
	cp -r data/debug.properties data/Dawn data/guam data/GVWin tmp/guam/data
	mkdir tmp/guam/tmp
	cp -r tmp/build tmp/guam/tmp
	cp    data/windows/* tmp/guam
	tar cfz tmp/guam.taz -C tmp guam
	cp tmp/guam.taz /mnt/hgfs/raspi


#
# BUILD FOR NATIVE
#
qmake:
	(cd src/mesa;          qmake)
	(cd src/simple-opcode; qmake)
	(cd src/agent;         qmake)
	(cd src/trace;         qmake)
	(cd src/util;          qmake)
	(cd src/test;          qmake)
	(cd src/guam-headless; qmake)
	(cd src/guam;          qmake)
	(cd src/floppy;        qmake)
	qmake --version


#
# BUILD FOR RASPBERRY PI
#
qmake-raspi-qt4:
	(cd src/mesa;          qmake -spec ../../data/raspi/qt4/raspi)
	(cd src/simple-opcode; qmake -spec ../../data/raspi/qt4/raspi)
	(cd src/agent;         qmake -spec ../../data/raspi/qt4/raspi)
	(cd src/util;          qmake -spec ../../data/raspi/qt4/raspi)
	(cd src/test;          qmake -spec ../../data/raspi/qt4/raspi)
	(cd src/guam-headless; qmake -spec ../../data/raspi/qt4/raspi)
	(cd src/guam;          qmake -spec ../../data/raspi/qt4/raspi)
	
mount-raspi:
	sudo mkdir -p /mnt/raspi
	sudo mount -o ro,loop,offset=62914560 /mnt/hgfs/raspi/2014-06-20-wheezy-raspbian.img /mnt/raspi

umount-raspi:
	sudo umount /mnt/raspi

tar-raspi: tar
	cp tmp/guam.taz /mnt/hgfs/raspi


#
# BUILD FOR WIN32
#
qmake-win32:
	(cd src/mesa;          $(MXE_QMAKE))
	(cd src/simple-opcode; $(MXE_QMAKE))
	(cd src/agent;         $(MXE_QMAKE))
	(cd src/util;          $(MXE_QMAKE))
	(cd src/test;          $(MXE_QMAKE))
	(cd src/guam-headless; $(MXE_QMAKE))
	(cd src/guam;          $(MXE_QMAKE))

run-guam.exe: guam
	echo -n >tmp/debug.log
	tmp/build/guam/guam.exe

run-guam-headless.exe: guam-headless
	echo -n >tmp/debug.log
	tmp/build/guam-headless/guam-headless.exe

run-test.exe: test
	echo -n >tmp/debug.log
	tmp/build/test/test.exe
