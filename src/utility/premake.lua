package = newpackage()
package.name = "utilitycode"
package.language = "c++"
package.kind = "lib"

package.files = {
  matchfiles("*.h","*.cpp"),
}

package.buildflags = {"extra-warnings","fatal-warnings"}

package.includepaths = { ".." }
