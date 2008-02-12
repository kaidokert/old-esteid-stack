
DLSRC=testdownload.cpp \
	../utility/opensslObj.cpp ../cardlib/DynamicLibrary.cpp

CTSRC=testctapi.cpp \
	../cardlib/CTAPIManager.cpp ../cardlib/DynamicLibrary.cpp \
	../cardlib/CardBase.cpp
	
CXXFLAGS=-I. -I.. -g
LDFLAGS=-ldl

all: testdownload testctapi

testdownload: $(DLSRC)
	$(CXX) $(CXXFLAGS) $(LDFLAGS) -o $@ $^

testctapi: $(CTSRC)
	$(CXX) $(CXXFLAGS) $(LDFLAGS) -o $@ $^