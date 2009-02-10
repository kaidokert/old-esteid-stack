project "esteidpkcs11"
  language "C++"
  kind     "SharedLib"
  files  { "*.h","*.cpp" }
  includedirs { "..","/usr/include/PCSC" }
  if os.is("windows") then
	includedirs "pkcs11"
  end
  links { "cardlib" ,"utility"}

