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

BOARDDEFINES=
CXX=g++
BROWSER=firefox
DEBUGFLAGS=-g
WARNINGFLAGS=-Wall -Werror -Wextra

VPATH=tests src

.PRECIOUS: test1

OBJS=\
IrReader.o           IrSenderNonMod.o     IrWidget.o             Rc5Decoder.o \
IrReceiver.o         IrSenderPwm.o        IrWidgetAggregating.o  Rc5Renderer.o \
IrReceiverPoll.o     IrSenderSimulator.o  MultiDecoder.o \
IrReceiverSampler.o  IrSequence.o         Nec1Decoder.o \
IrSender.o           IrSignal.o           Nec1Renderer.o         Pronto.o

libInfrared.a: $(OBJS)
	$(AR) rs $@ $(OBJS)

%.o: %.cpp
	$(CXX) -Isrc -std=c++11 $(BOARDDEFINES) $(WARNINGFLAGS) $(OPTIMIZEFLAGS) $(DEBUGFLAGS) -c $<

test%: test%.o libInfrared.a
	$(CXX) -o $@ $< -L. -lInfrared
	./$@

doc:
	doxygen
	$(BROWSER) api-doc/index.html

gh-pages:
	tools/update-gh-pages.sh

clean:
	rm -rf *.a *.o api-doc xml test1 gh-pages

spotless: clean
	rm -rf keywords.txt

build-tests:

test: test1

xml/index.xml: $(wildcard src/*)
	$(DOXYGEN) $(DOXYFILE)

keywords.txt: xml/index.xml
	$(XSLTPROC) $(TRANSFORMATION) $< > $@

all: test doc keywords.txt

.PHONY: clean spotless
