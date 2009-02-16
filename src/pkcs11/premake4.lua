project "esteidpkcs11"
  location ("../../build/" .. _ACTION)
  language "C++"
  kind     "SharedLib"
  files  { "*.h","*.cpp" }
  includedirs { "..","/usr/include/PCSC" }
  if os.is("windows") then
	includedirs "pkcs11"
  end
  links { "cardlib" ,"utility"}

