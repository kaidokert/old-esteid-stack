bin_PROGRAMS = EsteidUtil

BUILTSTUFF=resources/esteid.xpm resources/logo_sk.xpm resources/auth_icon.xpm \
	resources/esteidbmp.cdata resources/digistampbmp.cdata resources/cert.cdata resources/cert_invalid.cdata

EsteidUtil_SOURCES = \
	wxprec.cpp		precompiled.h \
	EsteidUtil.cpp		\
	MainDialog.cpp		MainDialog.h	\
	MainDialog_2.cpp			\
	MainDialog_Image.cpp			\
	MainDialog_Pin.cpp			\
	AppSetting.cpp		AppSetting.h		\
	CertificateDialog.cpp	CertificateDialog.h	\
	DiagnosticsDialog.cpp	DiagnosticsDialog.h	\
	PersonalDataDialog.cpp	PersonalDataDialog.h	\
	PinDialog.cpp		PinDialog.h	\
	PinChangeDialog.cpp	PinChangeDialog.h	\
	PinChangeSecureDialog.cpp	PinChangeSecureDialog.h	\
				utility/netObj.h	\
	utility/opensslObj.cpp	utility/opensslObj.h	\
	utility/winInetObj.cpp	utility/winInetObj.h	\
	utility/asnObject.cpp	utility/asnObject.h	\
	utility/asnCertificate.cpp utility/asnCertificate.h	\
	utility/pkcs11module.cpp utility/pkcs11module.h	\
					cardlib/common.h	\
					cardlib/helperMacro.h	\
					cardlib/ManagerInterface.h \
	cardlib/PCSCManager.cpp 	cardlib/PCSCManager.h	\
	cardlib/CTAPIManager.cpp	cardlib/CTAPIManager.h	\
	cardlib/EstEidCard.cpp 		cardlib/EstEidCard.h	\
	cardlib/CardBase.cpp 		cardlib/CardBase.h	\
	cardlib/DynamicLibrary.cpp	cardlib/DynamicLibrary.h \
	cardlib/SCError.cpp		cardlib/SCError.h \
	$(BUILTSTUFF)

EXTRA_DIST = resources/esteidbmp.gif resources/digistampbmp.gif resources/cert.gif resources/cert_invalid.gif \
	resources/esteid.ico resources/logo_sk.ico resources/auth_icon.ico \
	resources/esteid.xpm resources/logo_sk.xpm resources/auth_icon.xpm \
	resources/auth_icon.gif resources/logo_grey.ico \
	MainDialog_Windows.cpp \
	EsteidUtil.rc EsteidUtil.vcproj EsteidUtil.cbp Setup.h \
	tests/Makefile  tests/precompiled.h  tests/testctapi.cpp  \
	tests/testdownload.cpp  tests/testreadcardid.cpp	

nodist_EsteidUtil_SOURCES =
#nodist_EsteidUtil_SOURCES = \
#	resources/esteidbmp.cdata resources/digistampbmp.cdata 
#	resources/esteid.xpm resources/logo_sk.xpm

$(PACKAGE)-MainDialog.$(OBJEXT): $(BUILTSTUFF)
MainDialog.$(OBJEXT): $(BUILTSTUFF)

CLEANFILES = $(nodist_EsteidUtil_SOURCES)

EsteidUtil_LDFLAGS =
EsteidUtil_LDADD = $(WX_LIBS)

#moved to configure.in
#EsteidUtil_CPPFLAGS = -I$(includedir)/PCSC -I$(oldincludedir)/PCSC
DiagnosticsDialog.o: CXXFLAGS+=-O0 #workaround stack trash in libc

et/EsteidUtil.mo: ../po/et.gmo
	mkdir -p et
	cp -v $< $@

ru/EsteidUtil.mo: ../po/ru.gmo
	mkdir -p ru
	cp -v $< $@

resources/%.cdata: resources/%.gif
	mkdir -p resources
	echo "static unsigned char "$*"_cdata[] = {" > $@
	od -v -t x1 < $< | sed -e 's/.......//' -e 's/ \(..\)/0x\1,/g' >> $@
	echo "};" >> $@

%.ppm: %.ico
	winicontoppm $< $@

%.xpm: %.ppm
	ppmtoxpm $< | sed -e 's/\[\] \= /_xpm[] = /' -e 's/resources\///'> $@
