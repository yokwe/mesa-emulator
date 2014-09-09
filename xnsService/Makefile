#
# Makefile
#

LOG4CPP_CONFIG    := data/debug.properties
export LOG4CPP_CONFIG


.PHONY: all qt5-default clean distclean
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


##
## courier related targets
##

JARDIR		:= ../Externals/jar
CLASSPATH	:= bin:$(JARDIR)/antlr-4.1-complete.jar:$(JARDIR)/hamcrest-core-1.3.jar:$(JARDIR)/junit-4.11.jar:$(JARDIR)/logback-classic-1.0.13.jar:$(JARDIR)/logback-core-1.0.13.jar:$(JARDIR)/slf4j-api-1.7.5.jar
COURIER_TARGET	:= Time2 Ethernet Datagram Echo Error ExpeditedCourier PacketExchange Routing Protocol


clear-antlr:
	rm -rf java-src/mesa/courier/antlr
	
gen-antlr:
	mkdir -p tmp/antlr
	rm   -rf tmp/antlr/*
	java -jar ../Externals/jar/antlr-4.1-complete.jar -o tmp/antlr -package mesa.courier.antlr -no-listener -visitor src-java/mesa/courier/antlr/Courier.g
	rm   -rf java-src/mesa/courier/antlr/*.java
	cp   -p  tmp/antlr/src-java/mesa/courier/antlr/*.java src-java/mesa/courier/antlr

gen-stub:
	java -cp $(CLASSPATH) mesa.app.GenStub $(COURIER_TARGET)

dump-courier:
	java -cp $(CLASSPATH) mesa.app.DumpCourier Time2
