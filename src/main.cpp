/***************************************************************************
 *   Kartesio is a program for calculating best fit curves with            * 
 *   experimental points using regression algorithms or neural networks.   *
 *                                                                         *
 *                   Kartesio has been created by                          *
 *                Luca Tringali, TRINGALINVENT@libero.it                   *
 *                                                                         *
 *                    Copyright 2011-2013 Luca Tringali                    *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 3 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.         *
 ***************************************************************************/

#include <QtGui/QApplication>

#include <kaboutdata.h>
#include <kcmdlineargs.h>
#include <klocale.h>
#include <kapplication.h>
#include <KUrl>

#include "mainwindow.h"

#define APP_VERSION "0.1.00"

static const char description[] =
    I18N_NOOP("A program for calculating best fit curves with experimental points.");

static const char version[] = APP_VERSION;


int main(int argc, char *argv[])
{

    KAboutData about("kartesio", 0, ki18n("Kartesio"), version, ki18n(description), KAboutData::License_GPL, ki18n("(C) 2011-2013 Luca Tringali"), KLocalizedString(), "https://projects.kde.org/projects/playground/edu/kartesio");
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
