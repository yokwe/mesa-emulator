#
# Makefile
#

LOG4CPP_CONFIG    := data/debug.properties
export LOG4CPP_CONFIG


.PHONY: all qt5-default clean distclean
.PHONY: run-app


all:
	(cd src/util;    make all)
	(cd src/courier; make all)
	(cd src/itp;     make all)
	(cd src/service; make all)
	(cd src/pex;     make all)
	(cd src/app;     make all)
	(cd src/test;    make all)
	
qmake:
	(cd src/util;    qmake)
	(cd src/courier; qmake)
	(cd src/itp;     qmake)
	(cd src/service; qmake)
	(cd src/pex;     qmake)
	(cd src/app;     qmake)
	(cd src/test;    qmake)
	qmake --version

qt5-default:
	sudo apt-get install qt5-default

clean:
	echo "PATH = $(PATH)"
	rm -rf tmp/build
	mkdir  -p tmp/build
	mkdir  tmp/build/util
	mkdir  tmp/build/courier
	mkdir  tmp/build/itp
	mkdir  tmp/build/service
	mkdir  tmp/build/pex
	mkdir  tmp/build/app
	mkdir  tmp/build/test

distclean: clean
	rm -f  src/*/Makefile
	rm -f  src/*/Makefile.Debug
	rm -f  src/*/Makefile.Release
	rm -f  src/*/object_script.*.Debug
	rm -f  src/*/object_script.*.Release


run-app: all
	echo -n >tmp/debug.log
	tmp/build/app/app

run-test:
	(cd src/util;    make all)
	(cd src/courier; make all)
	(cd src/test;    make all)
	echo -n >tmp/debug.log
	tmp/build/test/test
	
##
## courier related targets
##

JARDIR		:= ../Externals/jar
CLASSPATH	:= bin:$(JARDIR)/antlr-4.1-complete.jar:$(JARDIR)/hamcrest-core-1.3.jar:$(JARDIR)/junit-4.11.jar:$(JARDIR)/logback-classic-1.0.13.jar:$(JARDIR)/logback-core-1.0.13.jar:$(JARDIR)/slf4j-api-1.7.5.jar
#COURIER_TARGET	:= Time2 Ethernet Datagram Echo Error ExpeditedCourier PacketExchange Routing Protocol Boot SequencedPacket
COURIER_TARGET	:= Authentication1 Authentication2 Authentication3 Boot BulkData1 CHEntries CHProperty Clearinghouse2 Clearinghouse3 Datagram Echo Error Ethernet ExpeditedCourier Filing4 Filing5 Filing6 FilingSubset1 PacketExchange Printing3 Protocol Routing SequencedPacket Time2

clean-antlr:
	rm -rf src-java/mesa/courier/antlr/*.java
	
gen-antlr:
	mkdir -p tmp/antlr
	rm   -rf tmp/antlr/*
	java -jar ../Externals/jar/antlr-4.1-complete.jar -o tmp/antlr -package mesa.courier.antlr -no-listener -visitor src-java/mesa/courier/antlr/Courier.g
	rm   -rf src-java/mesa/courier/antlr/*.java
	cp   -p  tmp/antlr/src-java/mesa/courier/antlr/*.java src-java/mesa/courier/antlr

gen-stub:
	java -cp $(CLASSPATH) mesa.app.GenStub $(COURIER_TARGET)

dump-courier:
	java -cp $(CLASSPATH) mesa.app.DumpCourier Clearinghouse2

