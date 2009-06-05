bin_PROGRAMS = esteidutil

AM_CXXFLAGS=-Wall -Werror -Wno-long-long #-pedantic 

BUILTSTUFF=resources/esteid.xpm resources/logo_sk.xpm resources/auth_icon.xpm \
	resources/esteidbmp.cdata resources/digistampbmp.cdata resources/cert.cdata resources/cert_invalid.cdata

esteidutil_SOURCES = \
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
				utility/precompiled.h	\
				utility/netObj.h	\
	utility/opensslObj.cpp	utility/opensslObj.h	\
	utility/winInetObj.cpp	utility/winInetObj.h	\
	utility/asnObject.cpp	utility/asnObject.h	\
	utility/asnCertificate.cpp utility/asnCertificate.h	\
	utility/pkcs11module.cpp utility/pkcs11module.h	\
	utility/converters.cpp   utility/converters.h \
	utility/logger.cpp	utility/logger.h \
	utility/monitorThread.cpp utility/monitorThread.h \
	utility/threadObj.cpp	utility/threadObj.h\
	utility/pinDialog.cpp	utility/pinDialog.h \
				utility/pinDialog_res.h \
					cardlib/common.h	\
					cardlib/helperMacro.h	\
					cardlib/ManagerInterface.h \
	cardlib/PCSCManager.cpp 	cardlib/PCSCManager.h	\
	cardlib/CTAPIManager.cpp	cardlib/CTAPIManager.h	\
	cardlib/EstEidCard.cpp 		cardlib/EstEidCard.h	\
	cardlib/EstEidCardMaintainer.cpp cardlib/EstEidCardMaintainer.h	\
	cardlib/CardBase.cpp 		cardlib/CardBase.h	\
	cardlib/DynamicLibrary.cpp	cardlib/DynamicLibrary.h \
	cardlib/SCError.cpp		cardlib/SCError.h \
	cardlib/SIMCard.cpp		cardlib/SIMCard.h \
	cardlib/SmartCardManager.cpp	cardlib/SmartCardManager.h \
	cardlib/types.h			cardlib/types.cpp \
	cardlib/precompiled.h
	$(BUILTSTUFF)

EXTRA_DIST = resources/esteidbmp.gif resources/digistampbmp.gif resources/cert.gif resources/cert_invalid.gif \
	resources/esteid.ico resources/logo_sk.ico resources/auth_icon.ico \
	resources/esteid.xpm resources/logo_sk.xpm resources/auth_icon.xpm \
	resources/auth_icon.gif resources/logo_grey.ico \
	MainDialog_Windows.cpp \
	EsteidUtil.rc Setup.h \
	tests/precompiled.h  tests/testctapi.cpp  \
	tests/testdownload.cpp  tests/testreadcardid.cpp \
	tests/precompiled.cpp tests/testcardmanager.cpp \
	tests/testcert.cpp tests/testlogger.cpp tests/testmonitorthread.cpp \
	tests/testpindialog.cpp \
	cardlib/Doxyfile \
	premake4.lua cardlib/premake4.lua utility/premake4.lua tests/premake4.lua

nodist_esteidutil_SOURCES =
#nodist_esteidutil_SOURCES = \
#	resources/esteidbmp.cdata resources/digistampbmp.cdata 
#	resources/esteid.xpm resources/logo_sk.xpm

$(PACKAGE)-MainDialog.$(OBJEXT): $(BUILTSTUFF)
MainDialog.$(OBJEXT): $(BUILTSTUFF)

CLEANFILES = $(nodist_esteidutil_SOURCES) resources/*.cdata

esteidutil_LDFLAGS =
esteidutil_LDADD = $(WX_LIBS)

#moved to configure.in
#esteidutil_CPPFLAGS = -I$(includedir)/PCSC -I$(oldincludedir)/PCSC
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
