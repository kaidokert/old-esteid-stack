#!/bin/bash -x

g++ -DHAVE_CONFIG_H -I. -I.. -I.. -I../pkcs11 -g -O2 -I/usr/include/PCSC -c esteidpkcs11.cpp  -fPIC -DPIC -o .libs/esteidpkcs11.o

g++ -DHAVE_CONFIG_H -I. -I.. -I.. -I../pkcs11 -g -O2 -I/usr/include/PCSC -c PKCS11Context.cpp  -fPIC -DPIC -o .libs/PKCS11Context.o

g++ -DHAVE_CONFIG_H -I. -I.. -I.. -I../pkcs11 -g -O2 -I/usr/include/PCSC -c precompiled.cpp  -fPIC -DPIC -o .libs/precompiled.o

g++ -shared .libs/esteidpkcs11.o .libs/PKCS11Context.o .libs/precompiled.o ../cardlib/libcardlib.a ../utility/libutility.a -o libesteidpkcs11.so