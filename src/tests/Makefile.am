bin_PROGRAMS = testcardmanager testcert testctapi testdownload \
      testlogger testmonitorthread testreadcardid testpkcs #testpindialog 

INCLUDES = -I${oldincludedir}/PCSC -I$(top_srcdir) -I$(top_srcdir)/src 
LDADD=@top_builddir@/src/utility/libutility.la @top_builddir@/smartcard++/libsmartcard++-1.0.la

testcardmanager_SOURCES = testcardmanager.cpp precompiled.h
testcert_SOURCES = testcert.cpp
testctapi_SOURCES = testctapi.cpp
testdownload_SOURCES = testdownload.cpp
testlogger_SOURCES = testlogger.cpp
testmonitorthread_SOURCES = testmonitorthread.cpp
#testpindialog_SOURCES = testpindialog.cpp
testreadcardid_SOURCES = testreadcardid.cpp
testpkcs_SOURCES = testpkcs.cpp

EXTRA_DIST=premake4.lua