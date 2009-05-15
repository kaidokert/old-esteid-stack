module("wx")

function createWxProject(pm,name,fileset)
	pm.project(name)
	language "C++"
	kind     "WindowedApp"
	files = fileset
	pchheader "precompiled.h"
	pchsource "precompiled.cpp"
	flags { "WinMain" }
	includedirs { 
		  path.getabsolute(os.getenv("WXWIN") .. "/lib/vc_lib/mswd")
		, path.getabsolute(os.getenv("WXWIN") .. "/include" )
		}
	libdirs {
		  os.getenv("WXWIN") .. "/lib/vc_lib"
		}
	links {
		"wxbase" .. os.getenv("WXLIBVER") .."d" ,
		"wxmsw"  .. os.getenv("WXLIBVER") .."d_core" ,
		"wxmsw"  .. os.getenv("WXLIBVER") .."d_adv" ,
		"wxjpegd" , "comctl32" , "Rpcrt4"
		}
	configuration { "Debug*" }
		defines { "_DEBUG", "DEBUG" }
		flags   { "Symbols" }
	premake.buildconfigs()
	return premake.CurrentContainer
end