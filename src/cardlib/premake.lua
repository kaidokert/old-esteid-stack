package = newpackage()
package.name = "cardlib"
package.language = "c++"
package.kind = "lib"

package.files = {
  matchfiles("*.h","*.cpp"),
}

if (linux) then
	package.includepaths = { "/usr/include/PCSC" }
end