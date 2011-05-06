#include <QtGui/QApplication>

#include <kaboutdata.h>
#include <kcmdlineargs.h>
#include <klocale.h>
#include <kapplication.h>
#include <KUrl>

#include "mainwindow.h"

#define APP_VERSION "0.9.80"

static const char description[] =
    I18N_NOOP("A program to calculate best fit curves");

static const char version[] = APP_VERSION;


int main(int argc, char *argv[])
{

    KAboutData about("kartesio", 0, ki18n("Kartesio"), version, ki18n(description), KAboutData::License_GPL, ki18n("(C) 2010-2011 Luca Tringali"), KLocalizedString(), "http://edu.kde.org/kartesio");
    about.addAuthor( ki18n("Luca Tringali"), KLocalizedString(), "TRINGALINVENT@libero.it" );
    //about.addCredit(ki18n("your name here"),ki18n("What you have done"));

    KCmdLineArgs::init(argc, argv, &about);

    KCmdLineOptions options;
    options.add("+[file]", ki18n("Document to open"));
    KCmdLineArgs::addCmdLineOptions(options);

    KApplication a;

    MainWindow *w = new MainWindow();
    w->show();

    KCmdLineArgs *args = KCmdLineArgs::parsedArgs();
    if (args->count())
    {
        w->Openarg(args->url(0).url());
    }

    return a.exec();
}
