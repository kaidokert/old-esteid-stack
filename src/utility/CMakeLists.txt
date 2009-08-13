include_directories (../../ /usr/include/PCSC/)

set(LIBUTILITY_HEADERS 
	asnObject.h		asnCertificate.h
	)

set(LIBUTILITY_SOURCES 
	asnObject.cpp		asnCertificate.cpp
	)


add_library (utility STATIC ${LIBUTILITY_SOURCES} ${LIBUTILITY_HEADERS}) 
