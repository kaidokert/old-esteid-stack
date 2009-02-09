project "esteidpkcs11"
  language "C++"
  kind     "SharedLib"
  files  { "*.h","*.cpp" }
  includedirs { "..","pkcs11","/usr/include/PCSC" }
  links { "cardlib" ,"utility"}

