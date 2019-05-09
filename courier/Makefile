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
	(cd src/test;    make all)
	(cd src/courier-app; make all)
	
qmake:
	(cd src/util;    qmake)
	(cd src/courier; qmake)
	(cd src/test;    qmake)
	(cd src/stub;    qmake)
	(cd src/courier-app; qmake)
	
	qmake --version

qt5-default:
	sudo apt-get install qt5-default

clean:
	echo "PATH = $(PATH)"
	rm -rf tmp/build
	mkdir  -p tmp/build
	mkdir  tmp/build/util
	mkdir  tmp/build/courier
	mkdir  tmp/build/test
	mkdir  tmp/build/stub
	mkdir  tmp/build/courier-app

distclean: clean
	rm -f  src/*/Makefile
	rm -f  src/*/Makefile.Debug
	rm -f  src/*/Makefile.Release
	rm -f  src/*/object_script.*.Debug
	rm -f  src/*/object_script.*.Release
	rm -f  src/*/.qmake.stash

run-courier-app:
	(cd src/util;    make all)
	(cd src/courier; make all)
	(cd src/stub;    make all)
	(cd src/courier-app; make all)
	echo -n >tmp/debug.log
	tmp/build/courier-app/courier-app

run-test:
	(cd src/util;    make all)
	(cd src/courier; make all)
	(cd src/stub;    make all)
	(cd src/test;    make all)
	echo -n >tmp/debug.log
	tmp/build/test/test

stub:
	(cd src/util;    make all)
	(cd src/courier; make all)
	(cd src/stub; make all)

##
## courier related targets
##

JARDIR		:= ../Externals/jar
CLASSPATH	:= bin:$(JARDIR)/antlr-4.1-complete.jar:$(JARDIR)/hamcrest-core-1.3.jar:$(JARDIR)/junit-4.11.jar:$(JARDIR)/logback-classic-1.0.13.jar:$(JARDIR)/logback-core-1.0.13.jar:$(JARDIR)/slf4j-api-1.7.5.jar
COURIER_TARGET	:= Ethernet IDP
COURIER_TARGET	+= Echo Error PacketExchange Routing Boot SequencedPacket Time
COURIER_TARGET	+= Protocol ExpeditedCourier
COURIER_TARGET	+= Time2 Authentication1 Authentication2 Authentication3 BulkData1 CHEntries0 Clearinghouse2 Clearinghouse3
#COURIER_TARGET	+= Filing4 Filing5 Filing6 FilingSubset1

COURIER_TARGET  += T_PRE T_CHO T_SEQ T_ARR


clean-antlr:
	rm -rf src-java/mesa/courier/antlr/*.java
	
gen-antlr:
	mkdir -p tmp/antlr
	rm   -rf tmp/antlr/*
	java -jar ../Externals/jar/antlr-4.1-complete.jar -o tmp/antlr -package mesa.courier.antlr -no-listener -visitor src-java/mesa/courier/antlr/Courier.g
	rm   -rf src-java/mesa/courier/antlr/*.java
	cp   -p  tmp/antlr/src-java/mesa/courier/antlr/*.java src-java/mesa/courier/antlr

gen-stub:
	rm   -rf src/stub/*.cpp src/stub/*.h
	java -cp $(CLASSPATH) mesa.app.GenStub $(COURIER_TARGET)
#	cp   -p  src/stub-custom/*.cpp src/stub-custom/*.h src/stub

dump-courier:
	java -cp $(CLASSPATH) mesa.app.DumpCourier IDP

