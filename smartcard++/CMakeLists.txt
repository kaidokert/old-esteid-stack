include_directories (../ ./ ../.. ../../src /usr/include/PCSC/)

set(LIBSMARTCARD_HEADERS 
	smartcard++.h 		CardBase.h 
	common.h 		CTAPIManager.h 
	DynamicLibrary.h 	helperMacro.h 
	ManagerInterface.h 	PCSCManager.h 
	precompiled.h 		SCError.h 
	SmartCardManager.h 	locked_allocator.h 
	types.h)

set(LIBSMARTCARD_SOURCES 
	CardBase.cpp 		CTAPIManager.cpp 
	DynamicLibrary.cpp 	PCSCManager.cpp 
	SCError.cpp 		SmartCardManager.cpp 
	locked_allocator.cpp 	types.cpp)

set(LIBESTEID_HEADERS esteid/EstEidCard.h esteid/EstEidCardMaintainer.h)
set(LIBESTEID_SOURCES esteid/EstEidCard.cpp esteid/EstEidCardMaintainer.cpp)

set(LIBSIM_HEADERS sim/SIMCard.h)
set(LIBSIM_SOURCES sim/SIMCard.cpp)

add_library (sim ${LIBSIM_SOURCES} ${LIBSIM_HEADERS}) 
add_library (esteid ${LIBESTEID_SOURCES} ${LIBESTEID_HEADERS})

add_library (smartcard++ SHARED ${LIBSMARTCARD_SOURCES} ${LIBSMARTCARD_HEADERS}) 

target_link_libraries (smartcard++ sim) 
target_link_libraries (smartcard++ esteid)

install(TARGETS smartcard++ sim esteid DESTINATION esteidutil/lib)