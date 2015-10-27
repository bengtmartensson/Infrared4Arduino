# Makefile for creating stuff on host.
# Use Arduino IDE for compiling for Arduino

CXX=g++
BROWSER=firefox
DEBUGFLAGS=-g
WARNINGFLAGS=-Wall -Werror

VPATH=tests

OBJS=\
IrReader.o           IrSenderNonMod.o     IrWidget.o             Rc5Decoder.o \
IrReceiver.o         IrSenderPwm.o        IrWidgetAggregating.o  Rc5Renderer.o \
IrReceiverPoll.o     IrSenderSimulator.o  MultiDecoder.o \
IrReceiverSampler.o  IrSequence.o         Nec1Decoder.o \
IrSender.o           IrSignal.o           Nec1Renderer.o

libInfrared.a: $(OBJS)
	$(AR) rs $@ $(OBJS)

%.o: %.cpp
	$(CXX) -I. -std=c++11 $(WARNINGFLAGS) $(OPTIMIZEFLAGS) $(DEBUGFLAGS) -c $<

test%: test%.o libInfrared.a
	$(CXX) -o $@ $< -L. -lInfrared
	./$@

doc:
	doxygen
	$(BROWSER) html/index.html

clean:
	rm -rf *.a *.o html test1

build-tests:

test: test1

