project "testreadcardid"
  language "C++"
  kind     "ConsoleApp"
  files  { "testreadcardid.cpp" }
  includedirs { ".." }
  links { "cardlib" }
  configuration { "Debug*" }
    defines { "_DEBUG", "DEBUG" }
    flags   { "Symbols" }

project "testcardmanager"
  language "C++"
  kind     "ConsoleApp"
  files  { "testcardmanager.cpp" }
  includedirs { ".." }
  links { "cardlib" }
  configuration { "Debug*" }
    defines { "_DEBUG", "DEBUG" }
    flags   { "Symbols" }

project "testctapi"
  language "C++"
  kind     "ConsoleApp"
  files  { "testctapi.cpp" }
  includedirs { ".." }
  links { "cardlib" }
  configuration { "Debug*" }
    defines { "_DEBUG", "DEBUG" }
    flags   { "Symbols" }

project "testdownload"
  language "C++"
  kind     "ConsoleApp"
  files  { "testdownload.cpp" }
  includedirs { ".." }
  links { "cardlib","utility" }
  configuration { "Debug*" }
    defines { "_DEBUG", "DEBUG" }
    flags   { "Symbols" }
