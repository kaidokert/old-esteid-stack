
function createConfigs(extra_flags)
  doDebugConfig()
  if (extra_flags) then flags { extra_flags } end
  doReleaseConfig()
  if (extra_flags) then flags { extra_flags } end
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

function fixSolutionPaths(sln)
  for prj in premake.eachproject(sln) do
    --print(prj.name)
    for cfg in premake.eachconfig(prj) do								
    --  print("----" .. cfg.name)
    end
  end
end