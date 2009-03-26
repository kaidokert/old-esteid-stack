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
	links { "cardlib","utility" }
	if os.is("linux") then
		links "dl"
	end
	configuration { "Debug*" }
		defines { "_DEBUG", "DEBUG" }
		flags   { "Symbols" }
		if name == "testPinDialog" then flags { "WinMain" } end

	premake.buildconfigs()
	return premake.CurrentContainer
end

createSampleProject("testreadcardid",{"testreadcardid.cpp"})
createSampleProject("testcardmanager",{"testcardmanager.cpp"})
createSampleProject("testctapi",{"testctapi.cpp"})
createSampleProject("testdownload",{"testdownload.cpp"})
createSampleProject("testPinDialog",{"testpindialog.cpp","testpindialog.rc"})
