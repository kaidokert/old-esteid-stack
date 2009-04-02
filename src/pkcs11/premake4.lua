project "esteidpkcs11"
  location ("../../build/" .. _ACTION)
  language "C++"
  kind     "SharedLib"

  files  { "*.h","*.cpp" }
  includedirs { "..","/usr/include/PCSC" }
  if os.is("windows") then
	includedirs "pkcs11"
  end
  if os.is("macosx") then
        includedirs "mac_pkcs11_headers"
  end
    
  links { "cardlib" ,"utility"}
  configuration { "Debug*" }
    defines { "_DEBUG", "DEBUG"}
    flags   { "Symbols" }

