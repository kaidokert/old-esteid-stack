function createSampleProject(name,cppfile,guid)
	project(name)
	uuid(guid)
	location ("../../build/" .. _ACTION)
	language "C++"
	if name == "testPinDialog" then
		kind "WindowedApp"
	else
		kind "ConsoleApp"
	end
	files  { cppfile }
	includedirs { "../..","..","/usr/include/" }
	links { "utility","smartcard++" }
	if os.is("linux") then
		links {"dl","pthread"}
		if name == "testPinDialog" then
		  linkoptions  {"`pkg-config gtkmm-2.4 --libs`"}
		  buildoptions {"`pkg-config gtkmm-2.4 --cflags`"}
		end
	else
		if name == "testPinDialog" then
			flags "WinMain"
		end
	end
	if name == "testPinDialog" then 
	  createConfigs({ "WinMain" })
	else
	  createConfigs()
	end

	premake.buildconfigs()
	return premake.CurrentContainer
end

createSampleProject("testreadcardid",{"testreadcardid.cpp"},"094C61ED-A334-6E48-A572-1CD9817BBBA8")
createSampleProject("testcardmanager",{"testcardmanager.cpp"},"328985C1-E147-6548-8F0A-58A4D71BE61F")
createSampleProject("testctapi",{"testctapi.cpp"},"0B965702-CE50-0142-99EC-1EF04A8A0747")
createSampleProject("testdownload",{"testdownload.cpp"},"42A20BA8-4E2B-3F4F-BCBB-F1F0092F0610")
if os.is("windows") then
  createSampleProject("testPinDialog",{"testpindialog.cpp","testpindialog.rc"},"C64BEDFC-9570-4B40-BCFE-D69813F0253A")
else
  createSampleProject("testPinDialog",{"testpindialog.cpp"},"C64BEDFC-9570-4B40-BCFE-D69813F0253A")
end
createSampleProject("testMonitorThread",{"testmonitorthread.cpp"},"BE67BF46-173F-A54E-BE66-7FD11922695B")
createSampleProject("testlogger",{"testlogger.cpp"},"A61E7196-6B89-8F44-9DFF-F1EFC5AE6E8B")
createSampleProject("testcert",{"testcert.cpp"},"F83249DF-A06F-284E-B4CA-0623900D4541")
createSampleProject("testsign",{"testsign.cpp"},"C30767FF-8179-4185-9959-871E189ED8AF")
createSampleProject("testpkcs", {"testpkcs.cpp"},"6C566498-FBA7-46a2-9651-74ACBC0EFB2E")