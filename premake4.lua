require "common"

solution "esteid"
  location ("build/" .. _ACTION)
  platforms { "x32" , "x64" }
  configurations { "Debug", "Release" }

  if os.is("macosx") then
        includedirs { "/System/Library/Frameworks/PCSC.framework/Versions/A/Headers" }
  end
  if os.is("windows") then
		defines "WIN32"
  end

include "src/cardlib"
include "src/utility"
include "src/pkcs11"
include "src/tests"

premake.buildconfigs()
fixSolutionPaths(solution())
