project "testreadcardid"
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
  language "C++"
  kind     "ConsoleApp"
  files  { "testcardmanager.cpp" }
  includedirs { "..","/usr/include/PCSC" }
  links { "cardlib","dl" }
  if os.is("linux") then
    links "dl"
  end
  configuration { "Debug*" }
    defines { "_DEBUG", "DEBUG" }
    flags   { "Symbols" }

project "testctapi"
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
