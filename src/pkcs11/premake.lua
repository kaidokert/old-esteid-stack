package = newpackage()
package.name = "esteidpkcs11-1.0"
package.language = "c++"
package.kind = "dll"
package.links = { "cardlib"}

package.files = {
  matchfiles("*.h","*.cpp"),
}

package.includepaths = { "..","pkcs11" }

package.buildflags = {"extra-warnings","fatal-warnings"}
package.libpaths = {"../cardlib"}

if (linux) then
	table.insert(package.includepaths, "/usr/include/PCSC")
end