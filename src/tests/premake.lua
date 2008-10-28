package = newpackage()
package.name = "testreadcardid"
package.kind = "exe"
package.files = { "testreadcardid.cpp" }
package.includepaths = { ".." }
package.links = { "cardlib"}
if (linux) then
	table.insert(package.includepaths, "/usr/include/PCSC")
	table.insert(package.links, "dl")
end

package = newpackage()
package.name = "testctapi"
package.kind = "exe"
package.files = { "testctapi.cpp" }
package.includepaths = { ".." }
package.links = { "cardlib"}
if (linux) then
	table.insert(package.includepaths, "/usr/include/PCSC")
	table.insert(package.links, "dl")
end

package = newpackage()
package.name = "testcardmanager"
package.kind = "exe"
package.files = { "testcardmanager.cpp" }
package.includepaths = { ".." }
package.links = { "cardlib"}
if (linux) then
	table.insert(package.includepaths, "/usr/include/PCSC")
	table.insert(package.links, "dl")
end

package = newpackage()
package.name = "testdownload"
package.kind = "exe"
package.files = { "testdownload.cpp" }
package.includepaths = { ".." }
package.links = { "utilitycode","cardlib"}
if (linux) then
	table.insert(package.includepaths, "/usr/include/PCSC")
	table.insert(package.links, "dl")
end


