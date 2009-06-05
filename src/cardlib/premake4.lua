project "cardlib"
  uuid "D3E24F38-3540-5D47-9E80-C10FD6AF7A6D"
  location ("../../build/" .. _ACTION)
  language "C++"
  kind     "StaticLib"
  files  { "**.h", "**.cpp" }
  
  includedirs "."
  if os.is("windows") then
   defines { "WIN32" }
  else
   includedirs { "/usr/include","/usr/include/PCSC" }
  end

  createConfigs()
  