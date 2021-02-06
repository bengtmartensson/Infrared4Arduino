# Makefile for creating stuff on host.
# Use Arduino IDE for compiling for Arduino.

# The functions for generating keywords.txt requires KeywordsTxtGenerator,
# https://github.com/bengtmartensson/KeywordsTxtGenerator, to be installed in
# KEYWORD_TXT_GENERATOR_DIR.

ifneq ($(ARDUINO),)
$(error This Makefile is not for compiling target code, for that, use the Arduino IDE.)
endif

KEYWORD_TXT_GENERATOR_DIR = ../KeywordsTxtGenerator
DOXYGEN := doxygen
DOXYFILE :=  Doxyfile
XSLTPROC := xsltproc
TRANSFORMATION := $(KEYWORD_TXT_GENERATOR_DIR)/doxygen2keywords.xsl

CXX:=g++
BROWSER:=firefox
DEBUGFLAGS:=-g
WARNINGFLAGS:=-Wall -Werror -Wextra -pedantic

VPATH=tests src src/boards

GH_PAGES := gh-pages
VERSION_H := src/version.h

# Get VERSION from the version in library.properties
VERSION := $(subst version=,,$(shell grep version= library.properties))

ORIGINURL := $(shell git remote get-url origin)

.PRECIOUS: test1

OBJS=\
Board.o \
HashDecoder.o \
IrReader.o \
IrReceiver.o \
IrReceiverPoll.o \
IrReceiverSampler.o \
IrSender.o \
IrSender.o \
IrSenderNonMod.o \
IrSenderPwm.o \
IrSenderPwmHard.o \
IrSenderPwmSoft.o \
IrSenderPwmSoftDelay.o \
IrSenderPwmSpinWait.o \
IrSenderSimulator.o \
IrSequence.o \
IrSignal.o \
IrWidget.o \
IrWidgetAggregating.o \
MultiDecoder.o \
Nec1Decoder.o \
Nec1Renderer.o \
Pronto.o \
Rc5Decoder.o \
Rc5Renderer.o \
SIL.o

EXTRA_INCLUDES=\
InfraredTypes.h \
IrDecoder.h \
IrSenderNonMod.h \
IrSequenceReader.h \

NOT_EXPORTED_INCLUDE  = SIL.h Board.h

EXPORTED_INCLUDES := $(sort $(filter-out $(NOT_EXPORTED_INCLUDE), $(EXTRA_INCLUDES) $(subst .o,.h,$(OBJS))))

all: test doc keywords.txt library.properties

libInfrared.a: $(OBJS)
	$(AR) rs $@ $(OBJS)

%.o: %.cpp
	$(CXX) -Isrc -std=c++11 $(WARNINGFLAGS) $(OPTIMIZEFLAGS) $(DEBUGFLAGS) -c $<

test%: test%.o libInfrared.a
	$(CXX) -o $@ $< -L. -lInfrared
	./$@

release: push gh-pages tag deploy

push:
	git push

deploy:

version: $(VERSION_H)

$(VERSION_H): library.properties Makefile
	echo "Creating $(VERSION_H)"
	@echo "// This file was automatically generated from $<; do not edit." > $@
	@echo "#pragma once"						      >> $@
	@echo "/**"                                                           >> $@
	@echo " * Version of the current library."                            >> $@
	@echo " * Taken from the version in $<."                              >> $@
	@echo " */"                                                           >> $@
	@echo "#define VERSION \"$(VERSION)\""                                >> $@

api-doc/index.html xml/index.xml: $(wildcard src/*) $(VERSION_H) $(DOXYFILE) README.md
	GIT_VERSION=$(VERSION) $(DOXYGEN) $(DOXYFILE)

doc: api-doc/index.html
	$(BROWSER) $<

gh-pages: api-doc/index.html
	rm -rf $(GH_PAGES)
	git clone --depth 1 -b gh-pages ${ORIGINURL} ${GH_PAGES}
	( cd ${GH_PAGES} ; \
	cp -rf ../api-doc/* . ; \
	git add * ; \
	git commit -S -a -m "Update of API documentation" ; \
	git push )

tag:
	git checkout master
	git status
	git tag -s -a Version-$(VERSION) -m "Tagging Version-$(VERSION)"
	git push origin Version-$(VERSION)

clean:
	rm -rf *.a *.o api-doc xml test1 $(GH_PAGES) library.properties.tmp

spotless: clean
	rm -rf keywords.txt

build-tests:

test: test1

keywords.txt: xml/index.xml
	$(XSLTPROC) $(TRANSFORMATION) $< > $@

library.properties: Makefile
	sed -e "s/^includes=.*/includes=$(EXPORTED_INCLUDES:%=%,)/" -e s/,$$// $@ > $@.tmp
	mv $@.tmp $@

.PHONY: clean spotless doc
