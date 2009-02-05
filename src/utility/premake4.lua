project "utility"
  language "C++"
  kind     "StaticLib"
  files  { "**.h", "**.cpp" }
  includedirs { ".."}

  if os.is("windows") then
   defines { "WIN32" }
  end
