include_directories (../../ ../../src ../)

if(UNIX)
  include_directories (/usr/include/PCSC/)
endif(UNIX)

link_directories (${ESTEIDUTIL_BINARY_DIR}/smartcard++/ ${ESTEIDUTIL_BINARY_DIR}/src/utility) 
link_libraries (smartcard++ utility)

add_executable(testcardmanager testcardmanager.cpp precompiled.h)
add_executable(testcert testcert.cpp)
add_executable(testctapi testctapi.cpp)
add_executable(testlogger testlogger.cpp)
add_executable(testmonitorthread testmonitorthread.cpp)
add_executable(testreadcardid testreadcardid.cpp)
add_executable(testpkcs testpkcs.cpp)
add_executable(testpindialog testpindialog.cpp testpindialog.rc)

if(WIN32)
	set_target_properties(testpindialog PROPERTIES LINK_FLAGS "/subsystem:windows")
endif(WIN32)


install(TARGETS 
  testcardmanager testcert testctapi
  testlogger testmonitorthread
  testreadcardid testpkcs 
  DESTINATION esteidutil/bin)
