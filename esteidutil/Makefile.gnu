#This is out of date, used to build with it when automake wasnt there
DEBUG?=no
STATIC?=no

WXCONF=wx-config

ifneq ($(STATIC),no)
	WXCONF=~/Projects/wxWidgets-2.8.5/staticinst/bin/wx-config
endif

CXXFLAGS=`$(WXCONF) --cxxflags --debug=$(DEBUG) --static=$(STATIC)`
LDLIBS  =`$(WXCONF) --libs --debug=$(DEBUG) --static=$(STATIC)`
CXXFLAGS+=-I`pwd` -I/usr/include/PCSC -fexceptions

ifneq ($(DEBUG),no)
	CXXFLAGS+=-g -D_DEBUG -D__WXDEBUG_
else
	LDFLAGS+=-s
endif

all: esteidutil et/EsteidUtil.mo ru/EsteidUtil.mo

MainDialog.cpp: resources/esteid.xpm resources/logo_grey.xpm resources/esteidbmp.cdata

TRANSLATESOURCES = MainDialog.cpp MainDialog_2.cpp MainDialog_Pin.cpp PersonalDataDialog.cpp PinChangeDialog.cpp DiagnosticsDialog.cpp

SOURCES = $(TRANSLATESOURCES) EsteidUtil.cpp  CertificateDialog.cpp AppSetting.cpp
SOURCES+= cardlib/PCSCManager.cpp cardlib/EstEidCard.cpp cardlib/CardBase.cpp cardlib/DynamicLibrary.cpp cardlib/SCError.cpp
SOURCES+= cardlib/CTAPIManager.cpp
SOURCES+= utility/asnObject.cpp utility/asnCertificate.cpp

OBJECTS=$(SOURCES:.cpp=.o)

esteidutil: $(OBJECTS)
	$(LINK.cc) $^ $(LOADLIBES) $(LDLIBS) -o $@

%.ppm: %.ico
	winicontoppm $< $@

%.xpm: %.ppm
	ppmtoxpm $< | sed -e 's/\[\] \= /_xpm[] = /' -e 's/resources\///'> $@

%.mo: %.po
	msgfmt $< -o $@

et/EsteidUtil.po: $(TRANSLATESOURCES)
	xgettext --no-location -j -k_ $^ -o $@

et/EsteidUtil.mo: et/EsteidUtil.po

ru/EsteidUtil.po: $(TRANSLATESOURCES)
	xgettext --no-location -j -k_ $^ -o $@

ru/EsteidUtil.mo: ru/EsteidUtil.po

resources/esteidbmp.cdata: resources/esteidbmp.gif
	echo "static unsigned char esteidbmp_cdata[] = {" > $@
	od -v -t x1 < $< | sed -r -e 's/.{7}//' -e 's/ (.{2})/0x\1,/g' >> $@
	echo "};" >> $@

clean:
	rm -rf *.o *~ *.xpm *.ppm *.cdata esteidutil resources/*.cdata resources/*.xpm resources/*.ppm
