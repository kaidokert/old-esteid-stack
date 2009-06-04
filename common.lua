
function createConfigs(extra_flags)
  doDebugConfig()
  if (extra_flags) then flags { extra_flags } end
  doReleaseConfig()
  if (extra_flags) then flags { extra_flags } end
  premake.buildconfigs()
end

function createWxConfigs()
  doDebugConfig()
  wxConfig("d") --debug suffix
  doReleaseConfig()
  wxConfig("m") --minimal suffix
  premake.buildconfigs()
end

function ppath(strPath)
  if os.is("windows") then return "$(PlatformName)\\"  .. strPath else return path.getrelative(".", "./" ..strPath) end
end

function doReleaseConfig(cfname)
  configuration { "Release*" }
		prebuildcommands = { "echo Release bla bla bla" }
  targetdir(ppath("Release"))
  defines { "NDEBUG" }
  flags   { "Optimize","NoEditAndContinue","NoManifest","StaticRuntime" }
  flags   { "ExtraWarnings" }
end
function doDebugConfig(cfname)
  configuration { "Debug*" }
		prebuildcommands = { "echo Debug bla bla bla" }
  targetdir(ppath("Debug"))
  defines { "_DEBUG", "DEBUG" }
  flags   { "Symbols" }
end

function strEnv(env)
   local val = os.getenv(env)
   if (val == nil) then return "$(" .. env .. ")" else return val end
end

function wxConfig(suffix)
  if os.is("windows") then
    includedirs { 
      path.getabsolute(strEnv("WXWIN") .. "/lib/vc_lib/msw"..suffix) ,
	  path.getabsolute(strEnv("WXWIN") .. "/include" )
      }
    libdirs {
	  strEnv("WXWIN") .. "/lib/vc_lib"
	  }
    links {
	  "wxbase" .. strEnv("WXLIBVER") ..suffix ,
	  "wxmsw"  .. strEnv("WXLIBVER") ..suffix.."_core" ,
	  "wxmsw"  .. strEnv("WXLIBVER") ..suffix.."_adv" ,
	  "wxbase" .. strEnv("WXLIBVER") ..suffix.."_xml" ,
	  "wxbase" .. strEnv("WXLIBVER") ..suffix.."_net" ,
	  "wxjpeg"..suffix ,"wxexpat"..suffix}
  else
    buildoptions { "`wx-config --cxxflags`" }
    package.linkoptions = { "`wx-config --libs` " }
  end
end
		
function fixSolutionPaths(sln)
  for prj in premake.eachproject(sln) do
    --print(prj.name)
    for cfg in premake.eachconfig(prj) do								
    --  print("----" .. cfg.name)
    end
  end
end