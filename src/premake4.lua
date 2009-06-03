package.path = "?.lua;../?.lua"

project("EsteidUtil")
	location ("../build/" .. _ACTION)
	language "C++"
	kind     "WindowedApp"
	files  { "*.cpp" , "*.h" , "premake4.lua" }
	pchheader "precompiled.h"
	pchsource "wxprec.cpp"
	flags { "WinMain" }
	includedirs { 
		path.getabsolute(os.getenv("WXWIN") .. "/include" )
		, "."
		}
	libdirs {
		  os.getenv("WXWIN") .. "/lib/vc_lib"
		}
	links {
		"cardlib","utility"
		}
	if os.is("windows") then
		links { "crypt32" , "comctl32" , "Rpcrt4" }
		defines { "WIN32" }
		files { "*.rc" }
	end
	if os.is("linux") then
		links "dl"
	end
	doDebugConfig()
	wxConfig("d") --debug suffix
	doReleaseConfig()
	wxConfig("m") --minimal suffix
	premake.buildconfigs()
	
		