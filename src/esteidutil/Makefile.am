bin_PROGRAMS = esteidutil

AM_CXXFLAGS=-Wall -Wno-long-long $(WX_CXXFLAGS) #-Werror -pedantic 

INCLUDES = -I${oldincludedir}/PCSC -I$(top_srcdir) -I$(top_srcdir)/src 

esteidutil_LDADD=@top_builddir@/src/utility/libutility.la $(WX_LIBS) @top_builddir@/smartcard++/libsmartcard++-1.0.la

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
	MainDialog_Windows.cpp \
	$(BUILTSTUFF)

EXTRA_DIST = resources/esteidbmp.gif resources/digistampbmp.gif resources/cert.gif resources/cert_invalid.gif \
	resources/esteid.ico resources/logo_sk.ico resources/auth_icon.ico \
	resources/esteid.xpm resources/logo_sk.xpm resources/auth_icon.xpm \
	resources/auth_icon.gif resources/logo_grey.ico \
	EsteidUtil.rc Setup.h \
	premake4.lua

MainDialog.$(OBJEXT): $(BUILTSTUFF)

CLEANFILES = $(nodist_esteidutil_SOURCES) resources/*.cdata

DiagnosticsDialog.o: CXXFLAGS+=-O0 #workaround stack trash in libc

et/EsteidUtil.mo: ../po/et.gmo
	mkdir -p et
	cp -v $< $@

ru/EsteidUtil.mo: ../po/ru.gmo
	mkdir -p ru
	cp -v $< $@

#resources/%.cdata: resources/%.gif
.gif.cdata:
	mkdir -p resources
	echo "static unsigned char "$*"_cdata[] = {" | sed 's/resources\///' > $@
	od -v -t x1 < $< | sed -e 's/.......//' -e 's/ \(..\)/0x\1,/g' >> $@
	echo "};" >> $@

.ico.ppm:
	winicontoppm $< $@

.ppm.xmp:
	ppmtoxpm $< | sed -e 's/\[\] \= /_xpm[] = /' -e 's/resources\///'> $@
