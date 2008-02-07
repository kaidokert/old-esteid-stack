
DLSRC=testdownload.cpp \
	../utility/opensslObj.cpp ../cardlib/DynamicLibrary.cpp

CXXFLAGS=-I. -I..
LDFLAGS=-ldl

all: testdownload

testdownload: $(DLSRC)
	$(CXX) $(CXXFLAGS) $(LDFLAGS) -o $@ $^