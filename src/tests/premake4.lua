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
	if name == "testPinDialog" then 
	  createConfigs({ "WinMain" })
	else
	  createConfigs()
	end

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
createSampleProject("testlogger",{"testlogger.cpp"})
