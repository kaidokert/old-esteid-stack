project "cardlib"
  location ("../../build/" .. _ACTION)
  language "C++"
  kind     "StaticLib"
  files  { "**.h", "**.cpp" }

  includedirs { "/usr/include","/usr/include/PCSC" }

  if os.is("windows") then
   defines { "WIN32" }
  end

  configuration { "Debug*" }
	targetdir "Debug"
    defines { "_DEBUG", "DEBUG" }
    flags   { "Symbols" }

  configuration { "Release*" }
	targetdir "Release"
    defines { "NDEBUG" }
	flags   { "Optimize","NoEditAndContinue","NoManifest","StaticRuntime" }


  