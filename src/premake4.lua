project("esteidutil")
	location ("../build/" .. _ACTION)
	language "C++"
	kind     "WindowedApp"
	files  { "*.cpp" , "*.h" , "premake4.lua" , "wx/*", "EsteidUtil.rc" }
	pchheader "precompiled.h"
	pchsource "wxprec.cpp"
	flags { "WinMain" }
	includedirs { "."}
	links {
		"comctl32" , "Rpcrt4","Ws2_32", "crypt32"
		}
	links { "cardlib" ,"utility"}
	
	createWxConfigs()

