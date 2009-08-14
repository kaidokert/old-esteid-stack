include_directories (../../ ../../src ../ /usr/include/PCSC/)

set(LIBPKCS_HEADERS PKCS11Context.h precompiled.h)
set(LIBPKCS_SOURCES esteidpkcs11.cpp PKCS11Context.cpp precompiled.cpp)

link_directories (${ESTEIDUTIL_BINARY_DIR}/smartcard++/ ${ESTEIDUTIL_BINARY_DIR}/src/utility) 
link_libraries (smartcard++ utility dl)

add_library (esteidpkcs11 SHARED ${LIBPKCS_SOURCES} ${LIBPKCS_HEADERS}) 

install(TARGETS esteidpkcs11 DESTINATION esteidutil/lib)