# Makefile for creating stuff on host.
# Use Arduino IDE for compiling for Arduino

ifneq ($(ARDUINO),)
$(error This Makefile is not for compiling target code, for that, use the Arduino IDE.)
endif

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

clean:
	rm -rf *.a *.o api-doc test1

build-tests:

test: test1

all: test doc
