package.path = package.path .. ';../?.lua;../../?.lua'
require "common"

project "utility"
  uuid "9416835E-B059-1847-9B6F-6BC57776EFC9"
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
  createConfigs()
