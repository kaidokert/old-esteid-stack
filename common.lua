
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
  if os.is("windows") then return "$(PlatformName)\\"  .. strPath else return strPath end
end

function doReleaseConfig(cfname)
  configuration { "Release*" }
  targetdir(ppath("Release"))
  defines { "NDEBUG" }
  flags   { "Optimize","NoEditAndContinue","NoManifest","StaticRuntime" }
  flags   { "ExtraWarnings" }
end
function doDebugConfig(cfname)
  configuration { "Debug*" }
  targetdir(ppath("Debug"))
  defines { "_DEBUG", "DEBUG" }
  flags   { "Symbols" }
end

function wxConfig(suffix)
  includedirs { 
    path.getabsolute(os.getenv("WXWIN") .. "/lib/vc_lib/msw"..suffix) ,
	path.getabsolute(os.getenv("WXWIN") .. "/include" )
    }
  libdirs {
    os.getenv("WXWIN") .. "/lib/vc_lib"
	}
  links {
	"wxbase" .. os.getenv("WXLIBVER") ..suffix ,
	"wxmsw"  .. os.getenv("WXLIBVER") ..suffix.."_core" ,
	"wxmsw"  .. os.getenv("WXLIBVER") ..suffix.."_adv" ,
	"wxbase" .. os.getenv("WXLIBVER") ..suffix.."_xml" ,
	"wxbase" .. os.getenv("WXLIBVER") ..suffix.."_net" ,
	"wxjpeg"..suffix ,"wxexpat"..suffix}

end
		
function fixSolutionPaths(sln)
  for prj in premake.eachproject(sln) do
    --print(prj.name)
    for cfg in premake.eachconfig(prj) do								
    --  print("----" .. cfg.name)
    end
  end
end