
DLSRC=testdownload.cpp \
	../utility/opensslObj.cpp ../cardlib/DynamicLibrary.cpp

CTSRC=testctapi.cpp \
	../cardlib/CTAPIManager.cpp ../cardlib/DynamicLibrary.cpp \
	../cardlib/CardBase.cpp

READSRC=testreadcardid.cpp \
	../cardlib/PCSCManager.cpp ../cardlib/DynamicLibrary.cpp \
	../cardlib/CardBase.cpp ../cardlib/EstEidCard.cpp ../cardlib/SCError.cpp
	
CXXFLAGS=-I. -I.. -I/usr/include/PCSC -g
LDFLAGS=-ldl

all: testdownload testctapi testreadcardid

testdownload: $(DLSRC)
	$(CXX) $(CXXFLAGS) $(LDFLAGS) -o $@ $^

testctapi: $(CTSRC)
	$(CXX) $(CXXFLAGS) $(LDFLAGS) -o $@ $^

testreadcardid: $(READSRC)
	$(CXX) $(CXXFLAGS) $(LDFLAGS) -o $@ $^
