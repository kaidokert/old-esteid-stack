function createSampleProject(name,cppfile)
	project(name)
	location ("../../build/" .. _ACTION)
	language "C++"
	if name == "testPinDialog" then
		kind "WindowedApp"
	else
		kind     "ConsoleApp"
	end
	files  { cppfile }
	includedirs { "..","/usr/include/" }
	links { "utility","cardlib" }
	if os.is("linux") then
		links {"dl","pthread"}
		if name == "testPinDialog" then
		  linkoptions  {"`pkg-config gtkmm-2.4 --libs`"}
		  buildoptions {"`pkg-config gtkmm-2.4 --cflags`"}
		end
	else
		if name == "testPinDialog" then
			flags "WinMain"
		end
	end
	configuration { "Debug*" }
		defines { "_DEBUG", "DEBUG" }
		flags   { "Symbols" }
		if name == "testPinDialog" then flags { "WinMain" } end
		targetdir "Debug"
	configuration { "Release" }
		targetdir "Release"
		flags { "Optimize","NoEditAndContinue","NoManifest","StaticRuntime" }

	premake.buildconfigs()
	return premake.CurrentContainer
end

createSampleProject("testreadcardid",{"testreadcardid.cpp"})
createSampleProject("testcardmanager",{"testcardmanager.cpp"})
createSampleProject("testctapi",{"testctapi.cpp"})
createSampleProject("testdownload",{"testdownload.cpp"})
if os.is("windows") then
  createSampleProject("testPinDialog",{"testpindialog.cpp","testpindialog.rc"})
else
  createSampleProject("testPinDialog",{"testpindialog.cpp"})
end
createSampleProject("testMonitorThread",{"testmonitorthread.cpp"})