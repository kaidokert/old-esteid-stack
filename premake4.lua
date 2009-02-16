solution "esteid"
  location ("build/" .. _ACTION)
  configurations { "Debug", "Release" }

include "src/cardlib"
include "src/utility"
include "src/pkcs11"
include "src/tests"
