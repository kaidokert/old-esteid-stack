project "esteidcsp"
  uuid "DBCD4071-608C-4443-A440-45E15FE05BC3"
  location ("../build/" .. _ACTION)
  language "C++"
  kind     "SharedLib"

  files  { "*.h","*.cpp" }
  pchheader "precompiled.h"
  pchsource "precompiled.cpp"
  
  includedirs { "../src" }
  defines {"WIN32","_WINDOWS","_USRDLL","ESTEIDCSP_EXPORTS","_SCL_SECURE_NO_WARNINGS","_UNICODE","UNICODE"}
  links { "cardlib" ,"utility"}
  createConfigs()
