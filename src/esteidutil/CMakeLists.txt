
find_package(wxWidgets COMPONENTS base core adv)

if(wxWidgets_FOUND)

  #message( STATUS "wxW libs found: ${wxWidgets_LIBRARIES}" )
  include_directories (../../ ../../src ../ ${wxWidgets_INCLUDE_DIRS})
  add_definitions(${wxWidgets_DEFINITIONS})

  if(UNIX)
    include_directories (/usr/include/PCSC/)
  endif(UNIX)

  set(ESTEIDUTIL_SOURCES
    wxprec.cpp		EsteidUtil.cpp
    MainDialog.cpp	MainDialog_2.cpp
    MainDialog_Image.cpp	MainDialog_Pin.cpp
    AppSetting.cpp	CertificateDialog.cpp
    DiagnosticsDialog.cpp	PersonalDataDialog.cpp
    PinDialog.cpp	PinChangeDialog.cpp
    PinChangeSecureDialog.cpp	MainDialog_Windows.cpp)

  set(ESTEIDUTIL_HEADERS
    precompiled.h	MainDialog.h
    AppSetting.h	CertificateDialog.h
    DiagnosticsDialog.h	PersonalDataDialog.h
    PinDialog.h		PinChangeDialog.h
    PinChangeSecureDialog.h)

  if(WIN32)
    set(ESTEIDUTIL_SOURCES ${ESTEIDUTIL_SOURCES} EsteidUtil.rc)
  endif(WIN32)

  link_directories (${ESTEIDUTIL_BINARY_DIR}/smartcard++/ ${ESTEIDUTIL_BINARY_DIR}/src/utility)
  link_libraries (smartcard++ utility ${wxWidgets_LIBRARIES})

  if(WIN32)
    link_libraries (crypt32 comctl32 Rpcrt4)
    add_executable(esteidutil WIN32 ${ESTEIDUTIL_SOURCES} ${ESTEIDUTIL_HEADERS} ${ESTEIDUTIL_RESOURCES})
  elseif(UNIX)
    add_executable(esteidutil ${ESTEIDUTIL_SOURCES} ${ESTEIDUTIL_HEADERS} ${ESTEIDUTIL_RESOURCES})
  endif(WIN32)
  
  install(TARGETS esteidutil DESTINATION esteidutil/bin)

else(wxWidgets_FOUND)
  message("wxWidgets not found, esteidutil will not be built")
endif(wxWidgets_FOUND)