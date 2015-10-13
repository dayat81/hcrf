CXXFLAGS =	-O2 -g -Wall -fmessage-length=0

OBJS =		hcrf.o avptype.o diameter.o avp.o

LIBS =

TARGET =	hcrf

$(TARGET):	$(OBJS)
	$(CXX) -o $(TARGET) $(OBJS) $(LIBS)

all:	$(TARGET)

clean:
	rm -f $(OBJS) $(TARGET)
