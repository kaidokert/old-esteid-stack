package = newpackage()
package.name = "esteidutil"
package.language = "c++"
package.kind = "exe"

package.files = {
  matchfiles("*.h","*.cpp"),
}

package.includepaths = { "." }
package.links = { "cardlib", "utilitycode" }

package.pchheader = "precompiled.h"
package.pchsource = "wxprec.cpp"

if (linux) then
  table.insert(package.includepaths, "/usr/include/PCSC")
  package.buildoptions = { "`wx-config --cxxflags`" }
  package.linkoptions = { "`wx-config --libs`" }
end
