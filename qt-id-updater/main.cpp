#include <QApplication>

#include "idupdater.h"
#include "ProcessStarter.h"
#include "ScheduledUpdateTask.h"
#include <iostream>

using std::endl;
int printhelp() {
	std::cout 	<< "-help\t\t\t-this help"<< endl
			<< "-noautocheck\t\t-dont perform check at startup" << endl
			<< "-autoupdate\t\t-update automatically" << endl
			<< "-url http://foo.bar\t-use alternate url" << endl;
	return 0;
	}

int main(int argc, char *argv[])
{	
/*	CoInitialize(NULL);
	{
	ScheduledUpdateTask task;
	}
	CoUninitialize();*/
//	std::ofstream run("c:\\windows\\temp\\run.log",std::ios_base::app);
	QApplication app(argc, argv);
	QStringList args = app.arguments();
	if (args.contains("-help") || args.contains("-?") || args.contains("/?"))
		return printhelp();
	ProcessStarter proc(argv[0],"");
	bool quitNow = proc.Run();
//	run << "executed proc.run, result : " << quitNow << std::endl;
	QString url = "http://kaidokert.com/work/updater/";
	int urlAt = args.indexOf("-url");
	if (urlAt > 0 && urlAt < args.size()-1 ) 
		url = args[urlAt+1];
	idupdater form(url,
		!args.contains("-noautocheck"),args.contains("-autoupdate"));
	if (!quitNow) {
//		run << "showing form .. " << std::endl;
		form.show();
		}
	else {
//		run << "quitting " << std::endl;
		app.exit(0);
		return 0;
	}
	return app.exec();
}
