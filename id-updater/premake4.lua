package.path = "?.lua;../?.lua"

project("id-updater")
--	location ("../build/" .. _ACTION)
	language "C++"
	kind     "WindowedApp"
	files  { "*.cpp" , "*.h" , "premake4.lua" , "wx/*" }
	pchheader "precompiled.h"
	pchsource "precompiled.cpp"
	flags { "WinMain" }
	includedirs { 
		".",
		path.getabsolute(os.getenv("WXWIN") .. "/include" )
		}
	libdirs {
		  os.getenv("WXWIN") .. "/lib/vc_lib"
		}
	links {
		"comctl32" , "Rpcrt4","Ws2_32"
		}
	defines { "wxUSE_CHECKEDLISTCTRL=0" , "wxUSE_MD5=0"}
		
	configuration { "Debug*" }
		targetdir "Debug"
		defines { "_DEBUG", "DEBUG" }
		flags   { "Symbols" }
		includedirs { 
		  path.getabsolute(os.getenv("WXWIN") .. "/lib/vc_lib/mswd") }
		links {
			"wxbase" .. os.getenv("WXLIBVER") .."d" ,
			"wxmsw"  .. os.getenv("WXLIBVER") .."d_core" ,
			"wxmsw"  .. os.getenv("WXLIBVER") .."d_adv" ,
			"wxbase" .. os.getenv("WXLIBVER") .."d_xml" ,
			"wxbase" .. os.getenv("WXLIBVER") .."d_net" ,
			"wxjpegd" ,"wxexpatd"}
	configuration { "Release*" }
		targetdir "Release"
		defines { "NDEBUG"}
		includedirs { 
		  path.getabsolute(os.getenv("WXWIN") .. "/lib/vc_lib/msw") }
		links {
			"wxbase" .. os.getenv("WXLIBVER") .."" ,
			"wxmsw"  .. os.getenv("WXLIBVER") .."_core" ,
			"wxmsw"  .. os.getenv("WXLIBVER") .."_adv" ,
			"wxbase" .. os.getenv("WXLIBVER") .."_xml" ,
			"wxbase" .. os.getenv("WXLIBVER") .."_net" ,
			"wxjpeg" , "wxexpat"}
		