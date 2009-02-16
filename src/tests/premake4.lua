project "testreadcardid"
  location ("../../build/" .. _ACTION)
  language "C++"
  kind     "ConsoleApp"
  files  { "testreadcardid.cpp" }
  includedirs { "..","/usr/include/PCSC" }
  links { "cardlib" }
  if os.is("linux") then
    links "dl"
  end
  configuration { "Debug*" }
    defines { "_DEBUG", "DEBUG" }
    flags   { "Symbols" }

project "testcardmanager"
  location ("../../build/" .. _ACTION)
  language "C++"
  kind     "ConsoleApp"
  files  { "testcardmanager.cpp" }
  includedirs { "..","/usr/include/PCSC" }
  links { "cardlib" }
  if os.is("linux") then
    links "dl"
  end
  configuration { "Debug*" }
    defines { "_DEBUG", "DEBUG" }
    flags   { "Symbols" }

project "testctapi"
  location ("../../build/" .. _ACTION)
  language "C++"
  kind     "ConsoleApp"
  files  { "testctapi.cpp" }
  includedirs { "..","/usr/include/PCSC" }
  links { "cardlib" }
  if os.is("linux") then
    links "dl"
  end
  configuration { "Debug*" }
    defines { "_DEBUG", "DEBUG" }
    flags   { "Symbols" }

project "testdownload"
  location ("../../build/" .. _ACTION)
  language "C++"
  kind     "ConsoleApp"
  files  { "testdownload.cpp" }
  includedirs { "..","/usr/include/PCSC" }
  links { "cardlib","utility" }
  if os.is("linux") then
    links "dl"
  end
  configuration { "Debug*" }
    defines { "_DEBUG", "DEBUG" }
    flags   { "Symbols" }
