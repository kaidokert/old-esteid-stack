cmake_minimum_required(VERSION 2.6)
project (esteidutil)

add_subdirectory (smartcard++)
add_subdirectory (src/utility)
add_subdirectory (src/esteidpkcs11)
add_subdirectory (src/tests)

if(WIN32)
	#add_subdirectory (src/esteidutil)
	#add_subdirectory (src/windows/esteidcsp)
	#add_subdirectory (src/windows/csp_tool)
endif(WIN32)



