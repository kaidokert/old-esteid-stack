public release

IDUtil2 ActiveX component and demo application
==========================================================

version: 2.0
	rewrote the entire thing

version: 0.2
	Removed "demo" notifications.
	Added a demo application ( Demo\ subdirectory )
version: 0.1 
	build 200, fixed card reading error if the card is removed by user
		added error checking in JScript and VBA examples

General info:
=============
IDUtil is an ActiveX component that makes the data in the public data file of the ID Card available to applications
It can be used in Internet Explorer, Office environment (VBA), Windows scripts, Visual Basic and other environments supporting ActiveX.

Programming:
============
The component consists of: 
	Events:   CardInserted(), CardRemoved()
	Methods:  ReadCard() 
	Properties: 16 fields that match the data records on the ID card (first name, last name, personal code etc )
	(In Estonian: see the EstEID card usage instruction at  http://www.id.ee/files/EstEID%20kaardi%20kasutusjuhend.pdf )

Example scripts are bundled in the HTML file (for IE) and VBA macro (for Office applications).

Example application:
====================
Simple PowerPoint2000 presentation where a macro has been added which deals with the events of inserting and removing the card and changes personal greeting texts in the presentation.
To use, first launch the file install.htm in Internet Explorer that installs the component. If the installation was successful and you have a PC/SC card reader connected to the PC, you can test the component on the same page.
Next, open Tere.ppt in PowerPoint and accept macros at opening time (the macro is digitally signed by AS IT Grupp). Then run the macro StartID from Tools->Macro, this activates the IDUtil component.
Now, you can launch the slideshow (Slideshow->Start or the F5 key). When ID card is inserted, a greeting text is displayed.


Additional info:
http://www.jaja.ee
e-mail: infojaja.ee, kaidokert@gmail.com
