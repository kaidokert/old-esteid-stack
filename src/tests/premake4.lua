function createSampleProject(name,cppfile)
	location ("../../build/" .. _ACTION)
	project(name)
	language "C++"
	kind     "ConsoleApp"
	files  { cppfile }
	includedirs { "..","/usr/include/" }
	links { "cardlib","utility" }
	if os.is("linux") then
		links "dl"
	end
	configuration { "Debug*" }
		defines { "_DEBUG", "DEBUG" }
		flags   { "Symbols" }

	premake.buildconfigs()
	return premake.CurrentContainer
end

createSampleProject("testreadcardid","testreadcardid.cpp")
createSampleProject("testcardmanager","testcardmanager.cpp")
createSampleProject("testctapi","testctapi.cpp")
createSampleProject("testdownload","testdownload.cpp")
