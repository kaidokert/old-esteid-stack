
project("EsteidUtil")
	uuid "44232120-CC59-744C-9B22-AB6C0E6B75A3"
	location ("../../build/" .. _ACTION)
	language "C++"
	kind     "WindowedApp"
	files  { "*.cpp" , "*.h" , "premake4.lua" }
	pchheader "precompiled.h"
	pchsource "wxprec.cpp"
	flags { "WinMain" }
	includedirs { ".","..","../.." }
	links {
		"smartcard++","utility"
		}
	if isWindows() then
		links { "crypt32" , "comctl32" , "Rpcrt4" }
		files { "*.rc" }
	end
	if os.is("linux") then
		links "dl"
		prebuildcommands = { "echo bla bla bla" }
	end
	doDebugConfig()
	wxConfig("d") --debug suffix
	doReleaseConfig()
	wxConfig("m") --minimal suffix
	premake.buildconfigs()
		