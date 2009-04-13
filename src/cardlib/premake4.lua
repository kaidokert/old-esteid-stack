project "cardlib"
  location ("../../build/" .. _ACTION)
  language "C++"
  kind     "StaticLib"
  files  { "**.h", "**.cpp" }

  includedirs { "/usr/include","/usr/include/PCSC" }

  if os.is("windows") then
   defines { "WIN32" }
  end

  createConfigs()
  