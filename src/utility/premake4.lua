project "utility"
  location ("../../build/" .. _ACTION)
  language "C++"
  kind     "StaticLib"
  files  { "**.h", "**.cpp" }
  includedirs { ".."}

  if os.is("windows") then
   defines { "WIN32" }
  end
  if os.is("linux") then
    buildoptions {"`pkg-config gtkmm-2.4 --cflags`"}
  end
  configuration { "Debug*" }
    defines { "_DEBUG", "DEBUG" }
    flags   { "Symbols" }

  configuration { "Release*" }
    defines { "NDEBUG" }
    flags   { "Optimize" }

  