## sub directory

h_sources = asnCertificate.h  asnObject.h
cc_sources = asnCertificate.cpp  asnObject.cpp

#Install the headers in a versioned directory - e.g. examplelib-1.0:
library_includedir=$(includedir)/$(GENERIC_LIBRARY_NAME)-$(GENERIC_API_VERSION)/$(GENERIC_LIBRARY_NAME)/utility
library_include_HEADERS = $(h_sources)

INCLUDES = -I$(top_srcdir) -I..

noinst_LTLIBRARIES= libutility.la
libutility_la_SOURCES= $(h_sources) $(cc_sources) 
