#include <QApplication>

#include "idupdater.h"
#include "ProcessStarter.h"
#include <iostream>

using std::endl;
int printhelp() {
	std::cout 	<< "-help\t\t\t-this help"<< endl
			<< "-autocheck\t\t-perform check at startup" << endl
			<< "-autoupdate\t\t-update automatically" << endl
			<< "-url http://foo.bar\t-use alternate url" << endl;
	return 0;
	}

int main(int argc, char *argv[])
{
	QApplication app(argc, argv);
	QStringList args = app.arguments();
	if (args.contains("-help") || args.contains("-?") || args.contains("/?"))
		return printhelp();
	QString exec = args[0];
	ProcessStarter proc(argv[0],"-autocheck");
	if (proc.Run()) 
		return 1; // quit if we launched from service
	QString url = "http://kaidokert.com/work/updater/";
	int urlAt = args.indexOf("-url");
	if (urlAt > 0 && urlAt < args.size()-1 ) 
		url = args[urlAt+1];
	idupdater form(url,
		args.contains("-autocheck"),args.contains("-autoupdate"));
	form.show();
	return app.exec();
}
