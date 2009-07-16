'<script language="vbscript">
'MSXML handler script library
'Copyright IT Systems 2003


function sertByName(sname)
	set sertColl = createobject("dsiglite2.algorithm")
	for each e in sertColl.myCerts
		if (sname = e.displaystring)  and (e.keySpec = "SIGNATURE") then
			WSH.echo "Signing with " & e.displaystring
			set sertByName = e
			exit function
		end if
	next
	set sertByName = nothing
end function

function makeDoc(inputStr)
	set xmlDocs = createobject("msxml.domdocument")
	xmldocS.preserveWhiteSpace = true
	xmldocS.resolveExternals = false
	xmldocS.validateOnParse = false
	xmldocS.loadXML(inputStr)
	if xmldocS.parseError.errorCode <> 0  then
		WSH.echo "Problem loading '" & fail & "'" & xmldocS.parseError.reason
		set xmlDocs = nothing
	end if
	set makeDoc = xmldocS
end function

function getDoc(fail) 
	set xmlDocs = createobject("msxml.domdocument")
	xmldocS.preserveWhiteSpace = true
	xmldocS.resolveExternals = false
	xmldocS.validateOnParse = false
	xmldocS.async = true
	xmldocS.load fail
	while xmldocS.readyState <> 4
	wend
	if xmldocS.parseError.errorCode <> 0  then
		WSH.echo "Problem loading '" & fail & "'" & xmldocS.parseError.reason
		set xmlDocs = nothing
	end if
	set getDoc = xmldocS
end function

function do_reformat(file_in,file_out)
	set fso = createobject("scripting.filesystemobject")
	set striim = FSO.OpenTextFile(file_in,1,false,0)
	textXml = striim.readall
	striim.close

	set striim = FSO.OpenTextFile(file_out,2,true,0)
'	striim.write "<?xml version=""1.0"" encoding=""iso-8859-1""?>"	
	striim.write txt_reformat(textXml)
	striim.close
end function

'</script>