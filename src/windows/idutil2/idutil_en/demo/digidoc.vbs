'<script language="vbscript">
'Digidoc-XML 1.1 script library
'Copyright IT Systems 2003

'Ver 0.5.3
'SigningTime changed to UTC

Option explicit
const E_SRC = "script/dsig"
const D_SRC = "script/ddoc"

'Creates and returns a DOMDocument object, formatted as digidoc1.1 container
function DD_CreateContainer()
	Dim pXmlDoc
	set pXmlDoc = createobject("msxml.domdocument")

	pXmlDoc.loadXML "<SignedDoc format=""DIGIDOC-XML"" version=""1.1""></SignedDoc>"
	
	pXmlDoc.selectSingleNode("*").text = vbCrlf 'DDOC bug
	
	set DD_CreateContainer = pXmlDoc
end function

'Adds "EMBEDDED" XML datafile to digidoc container
function DD_addXmlDataNode(ddoc,dataNode,fileName)
	Dim rootNode
	dim errnum,errsrc,errdesc
	
	set rootNode = ddoc.selectsinglenode("*")

	'Find previous Datafile Id
	Dim node
	Dim nextDfID
	
	set node = nothing
	nextDfID = -1
	do
		nextDfID = nextDfID + 1
		set node = nodeById("#D" & nextDfID,ddoc)
	loop while not node is nothing
	
'	if nextDfID > 0 then 'DDOC bug
		set node = ddoc.createTextNode(vbCrLf)
		rootNode.appendChild(node)
'	end if

	set node = ddoc.createNode(1,"DataFile","")
	Dim dataFile
	set dataFile = rootNode.appendChild(node)
	dataFile.setAttribute "ContentType","EMBEDDED"
	dataFile.setAttribute "Filename",filename
	dataFile.setAttribute "Id","D" & nextDfID
	dataFile.setAttribute "MimeType","text/xml"
	
'FIXME
	dataFile.setAttribute "Size",Len(dataNode.xml)
	
	dataFile.text = vbCrlf 'DDOC bug
	
	on error resume next
	Dim dataNodeRoot
	set dataNodeRoot = dataNode.selectSingleNode("*")
	dataFile.appendChild dataNodeRoot

	errnum = err.number:errsrc = err.Source:errdesc = err.Description 
	on error goto 0
	if errnum <> 0 then
		err.Raise 205,D_SRC,"Unable to add xmlDataFile [" & errsrc & ":" & errdesc & "]"
		exit function
	end if
	
end function

'Creates a signature using specified certificate
function DD_signContainer(ddoc,certificate)
	Dim signNode,node,dataFileID
	dim errnum,errsrc,errdesc
	
	on error resume next
	set signNode = d_addSignature(ddoc,certificate)
	errnum = err.number:errsrc = err.Source:errdesc = err.Description 
	on error goto 0
	
	if errnum <> 0 then
		err.Raise 201,D_SRC,"Unable to add signature [" & errsrc & ":" & errdesc & "]"
		exit function
	end if
	
	set node = nothing
	dataFileID = 0
	do
		set node = nodeById("#D" & dataFileID,ddoc)
		if not node is nothing then
			on error resume next
			dsig_addReference signNode,"#D" & dataFileID
			errnum = err.number:errsrc = err.Source:errdesc = err.Description 
			on error goto 0
			if errnum <> 0 then
				err.Raise 202,D_SRC,"Unable to add reference to datafile " & dataFileID & "  [" & errsrc & ":" & errdesc & "]"
				exit function
			end if
		end if
		dataFileID = dataFileID + 1
	loop until node is nothing

	Dim rootNode	
	set rootNode = ddoc.selectsinglenode("*")
	rootNode.appendChild(signNode)

	Dim sigID
	sigID = signNode.getAttribute("Id")

	on error resume next
	dsig_addReference signNode,"#" & sigID & "-SignedProperties"
	errnum = err.number:errsrc = err.Source:errdesc = err.Description 
	on error goto 0
	if errnum <> 0 then
		err.Raise 203,D_SRC,"Unable to add reference to SignedProperties  [" & errsrc & ":" & errdesc & "]"
		exit function
	end if

	on error resume next
	dsig_sign signNode,certificate
	errnum = err.number:errsrc = err.Source:errdesc = err.Description 
	on error goto 0
	if errnum <> 0 then
		err.Raise 204,D_SRC,"Unable to sign container  [" & errsrc & ":" & errdesc & "]"
		exit function
	end if
end function

'digidoc helper functions

'Creates a <Signature> element within digidoc container
'Adds DSIG signature element, and creates an Object element
function d_addSignature(ddoc,certificate)
	Dim objCodec
	set objCodec = createobject("dsiglite2.algorithm")

	set d_addSignature = nothing
	
	'Figure out if previous Sig Id
	Dim node	
	Dim nextSigID
	set node = nothing
	nextSigID = -1
	do
		nextSigID = nextSigID + 1
		set node = nodeById("#S" & nextSigID,ddoc)
	loop while not node is nothing

	Dim newSignatureNode

	set newSignatureNode=dsig_createsig(ddoc,"S" & nextSigID,certificate)

	Dim qp,ssP,sp
	Dim n1,n2,n3,n4
	set Qp = appendNode("QualifyingProperties",appendNode("Object",newSignatureNode))
	set ssP = appendNode("SignedProperties",qP)
	ssp.setAttribute "Id","S" & nextSigId & "-SignedProperties"
	ssp.setAttribute "Target","#S" & nextSigId

		set Sp = appendNode("SignedSignatureProperties",ssP)
			Dim dateStr,timeStr,nT
			nT = makeUTC(now())
			dateStr = spacenum(year(nT),4) & "." & spacenum(month(nT),2) & "." & spacenum(day(nT),2)
			timeStr = spacenum(hour(nT),2) & ":" & spacenum(minute(nT),2) & ":" & spacenum(second(nT),2)
			set n1 = appendNode("SigningTime",Sp)
			n1.text = dateStr & "T" & timeStr & "Z"
			set n1 = appendNode("SigningCertificate",Sp)
				set n2 = appendNode("Cert",n1)
				n2.setAttribute "Id","S" & nextSigID & "-CERTINFO"
					set n3 = appendNode("CertDigest",n2)
						set n4 = appendNode("DigestMethod",n3)
						n4.setAttribute "Algorithm","http://www.w3.org/2000/09/xmldsig#sha1"
						set n4 = appendNode("DigestValue",n3)
						n4.text = objCodec.b64encode(certificate.thumbPrint)
					set n3 = appendNode("IssuerSerial",n2)
					n3.text = certificate.issuerSerial
	
			set n1 = appendNode("SignaturePolicyIdentifier",Sp)
				set n2 = appendNode("SignaturePolicyImplied",n1)
			if false then
				set n1 = appendNode("SignatureProductionPlace",Sp)
					set n2 = appendNode("City",n1)
					set n2 = appendNode("StateOrProvince",n1)
					set n2 = appendNode("PostalCode",n1)
					set n2 = appendNode("CountryName",n1)
				set n1 = appendNode("SignerRole",Sp)
					set n2 = appendNode("ClaimedRoles",n1)
						set n3 = appendNode("ClaimedRole",n2)
			end if

		set Sp = appendNode("SignedDataObjectProperties",ssP)

	set d_addSignature = newSignatureNode
end function

'XML DOM helper functions
function appendNode(nodeName,parentNode)
	Dim docObj,newNode

	set docObj = parentNode.selectSingleNode("/").ownerDocument
	if docObj is nothing then set docObj = parentNode.ownerDocument
	
	set newNode = parentNode.appendChild(docObj.createNode(1,nodeName,sig_NS))
	set appendNode = newNode
end function

function nodeById(strId,doc)
	Dim refNode
	set refNode = nothing
	if left(strId,1) = "#" then 
		strID = right(strId,len(strId)-1)
		if refnode is nothing then set refnode = doc.selectSingleNode("//*[@ID='" & strId & "']")
		if refnode is nothing then set refnode = doc.selectSingleNode("//*[@Id='" & strId & "']")
		if refnode is nothing then set refnode = doc.selectSingleNode("//*[@iD='" & strId & "']")
		if refnode is nothing then set refnode = doc.selectSingleNode("//*[@id='" & strId & "']")
	end if
	set nodeByID = refNode
end function

function spaceNum(nummer,places)
	spacenum = right(string(places,"0") & nummer,places)
end function

'DSIG functions
const sig_NS = "http://www.w3.org/2000/09/xmldsig#"

function dsig_createKeyInfo(sigNode,certificate)
	Dim objCodec,n1,keyInfNode,keyVal,rsaK,x509
	Dim modTxt,pubTxt,x5Txt
	set dsig_createKeyInfo = nothing
	
	set objCodec = createobject("dsiglite2.algorithm")
	
	modTxt = objCodec.b64encode(certificate.pubkeymodulus)
	pubTxt = objCodec.b64encode(certificate.pubkeyExponent)
	x5Txt = objCodec.b64encode(certificate.certBinary)
	
	if (len(modTxt) < 4 OR len(pubTxt) < 4 OR len(x5Txt) < 4 ) then 
		err.Raise 101,E_SRC,"Certificate cannot be read"
		exit function
	end if
	
	set keyInfNode = appendNode("KeyInfo",sigNode)
	set keyVal = appendNode("KeyValue",keyInfNode)
		set rsaK = appendNode("RSAKeyValue",keyVal)
			set n1 = appendNode("Modulus",rsaK)
			n1.text =  modTxt
			set n1 = appendNode("Exponent",rsaK)
			n1.text =  pubTxt
	set x509 = appendNode("X509Data",keyInfNode)
		set n1 = appendNode("X509Certificate",x509)
		n1.text = x5Txt

	set dsig_createKeyInfo = keyInfNode
end function

function dsig_createSig(xmldoc,sigid,certificate)
	Dim parentDoc
	Dim sigNode
	Dim certAlg 
	
	set dsig_createSig = nothing	
	certAlg = certificate.keyAlgorithmID
	if (len(certAlg)<4) then 
		err.Raise 101,E_SRC,"Certificate cannot be read"
		exit function
	end if
	
	set parentDoc = xmldoc.selectSingleNode("*").ownerDocument

	set sigNode = parentDoc.createNode(1,"Signature",sig_NS)
	sigNode.setAttribute "Id",Cstr(sigid)

	Dim infoNode,node
	set infoNode = appendNode("SignedInfo",sigNode)
	set node = appendNode("CanonicalizationMethod",infoNode)
	node.setAttribute "Algorithm","http://www.w3.org/TR/2001/REC-xml-c14n-20010315"
	set node = appendNode("SignatureMethod",infoNode)
	node.setAttribute "Algorithm",certAlg
	set node = appendNode("SignatureValue",sigNode)
	node.setAttribute "Id",sigid & "-SIG"

	on error resume next
	dsig_createKeyInfo sigNode,certificate

	if err.number<>0 then
		set dsig_createSig = nothing
		exit function
	end if
	on error goto 0
	
	set dsig_createSig = sigNode
end function

function dsig_addReference(pSigNode,uri)
	Dim processor,coder,signedInfo,refNode,node,digestNode
	Dim rootNode,calcNode
	Dim digestText
	Dim s_uri
	
	set dsig_addReference = nothing	
	s_uri = cstr(uri)
	
	set processor = createobject("dsiglite2.Canonic")
	set coder = createobject("dsiglite2.algorithm")
	set signedInfo = pSignode.selectsinglenode("SignedInfo")
	
	set rootNode = pSigNode.ownerDocument.selectsinglenode("*")
	set calcNode = nodeByID(s_uri,rootNode)
	if calcNode is nothing then
		err.Raise 102,E_SRC,"Referred URI cannot be found"
		exit function
	end if
	
	Dim canonizedDataStr,canonizedDataBytes,hashedDataBytes
	canonizedDataStr = processor.process(calcNode.xml)
	canonizedDataBytes = coder.strToByte(canonizedDataStr)
	hashedDataBytes = coder.sha1hash(canonizedDataBytes)
	digestText = coder.b64encode(hashedDataBytes)
	if len(digestText) < 4 then 
		err.Raise 103,E_SRC,"Unable to calculate referred digest"
		exit function
	end if

	set refNode = appendNode("Reference",signedInfo)
	refNode.setAttribute "URI","#" & s_uri
	set node = appendNode("DigestMethod",refNode)
	node.setAttribute "Algorithm","http://www.w3.org/2000/09/xmldsig#sha1"
	set digestNode = appendNode("DigestValue",refNode)
	digestNode.text = digestText
	
	set dsig_addReference = signedInfo.appendChild(refNode)
end function

function dsig_sign(signNode,certificate) 
	Dim processor,coder,sigValue,signedInfo
	Dim canonizedDataStr,canonizedDataBytes,rsaBytes
	Dim signedStr
	
	set dsig_sign = nothing	
	set processor = createobject("dsiglite2.canonic")
	set coder = createobject("dsiglite2.algorithm")
	
	set sigValue = signNode.selectSingleNode("SignatureValue")
	set signedInfo = signNode.selectSingleNode("SignedInfo")

	canonizedDataStr = processor.process(signedInfo.xml)
	canonizedDataBytes = coder.strToByte(canonizedDataStr)
	rsaBytes = coder.rsaSignature(canonizedDataBytes,certificate)
	signedStr = coder.b64encode(rsaBytes)
	if (len(signedStr) < 4) then
		err.Raise 104,E_SRC,"Error on signing operation"
		exit function
	end if
	
	sigValue.text = signedStr	set dsig_sign = signNode
end function

function makeUTC(ByVal vDate)
	Dim lngTimeOffset,boolVal
	
	if isdate(vdate) then
		vdate = cdate(vdate)
	else
		err.raise &h7001,"FormatUTC","Argument is not a valid date"
	end if
	
	boolVal = false
	on error resume next
	boolVal = isobject(document)
	on error goto 0
	if boolVal then
		Dim scr
		set scr = document.createElement("<script>")
		scr.setAttribute "language", "JScript"
		scr.text = vbcrlf & _
			"function JSTZOffset() {"  & vbcrlf & _
			"d = new Date();"  & vbcrlf & _
			"tz = d.getTimezoneOffset();"  & vbcrlf & _
			"return(tz);"  & vbcrlf & _
			"}"
		document.body.appendChild(scr)
			
		lngTimeOffset = JSTZOffset()
	else 
		lngTimeOffset = BTZOffset()
	end if
	makeUTC = DateAdd("n", lngTimeOffset, vDate)
end function

function BTZOffset()
	Dim shell,strValueName,tmpTimeOffset
	
	set shell = createobject("wscript.shell")
	strValueName = _
		"HKEY_LOCAL_MACHINE\SYSTEM\CurrentControlSet\Control\"_
		& "TimeZoneInformation\ActiveTimeBias"
	tmpTimeOffset = shell.regread(strValueName)

	if IsArray(tmpTimeOffset) then 'it's REG_BINARY w/ 2 bytes significant
		BTZOffset = tmpTimeOffset(0) + (tmpTimeOffset(1)*(2^8))
	else 'it's REG_DWORD
		BTZOffset = tmpTimeOffset
	end if
end function

'</script>