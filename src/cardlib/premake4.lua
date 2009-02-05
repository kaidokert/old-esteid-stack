project "cardlib"
  language "C++"
  kind     "StaticLib"
  files  { "**.h", "**.cpp" }

  if os.is("windows") then
   defines { "WIN32" }
  end

  configuration { "Debug*" }
    defines { "_DEBUG", "DEBUG" }
    flags   { "Symbols" }

  configuration { "Release*" }
    defines { "NDEBUG" }
    flags   { "Optimize" }

  