require "common"

solution "esteid"
  location ("build/" .. _ACTION)
  platforms { "x32" , "x64" }
  configurations { "Debug", "Release" }

  if os.is("macosx") then
        includedirs { "/System/Library/Frameworks/PCSC.framework/Versions/A/Headers" }
  end

  include "smartcard++"
  include "src/utility"
  include "src/tests"
  include "src/esteidpkcs11"
  include "src/esteidutil"
  if isWindows() then
	include "src/windows/esteidcsp"
	include "src/windows/csp_tool"
	include "src/windows/esteidcm"
  end

