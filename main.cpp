#include "diskmanagerwindow.h"
#include <QApplication>
#include <QFile>
#include <QDebug>
#include <qtranslator.h>
#include <qlocale.h>
#include <QTextCodec>
#include <QSharedMemory>

#include <unistd.h>
#include <sys/types.h>

#ifndef PREFIX
#define PREFIX QString("/usr")
#endif

int main(int argc, char *argv[])
{
    // Make sure we are running as root
    if (getuid() != 0) {
      qDebug("Error, needs to be run as root");
//      exit(1);
    }

   QApplication a(argc, argv);
   QSharedMemory shared("zfs-manager-52f7d4d8-4a0d-4e7a-b12e-2b402697e681");
   if( !shared.create( 512, QSharedMemory::ReadWrite) )
   {
   qWarning() << "Can't start more than one instance of the application.";
//   exit(0);
   }
   else {
   qDebug() << "Application started successfully.";
   }

    QTranslator translator;
    QLocale mylocale;
    QString langCode = mylocale.name();
    if ( ! QFile::exists( PREFIX + "/share/zfs-manager/i18n/ZManager_" + langCode + ".qm" ) )
      langCode.truncate(langCode.indexOf("_"));
    translator.load( QString("ZManager_") + langCode, PREFIX + "/share/zfs-manager/i18n/" );
    a.installTranslator( &translator );
    qDebug() << "Locale:" << langCode;
    QTextCodec::setCodecForLocale( QTextCodec::codecForName("UTF-8") ); //Force Utf-8 compliance
    
    diskmanagerWindow w;
    w.ProgramInit();
    w.show();

//    QObject::connect(&a, SIGNAL(InputsAvailable(QStringList)), &w, SLOT(slotSingleInstance()) );
//    a.connect( &a, SIGNAL( lastWindowClosed() ), &a, SLOT( quit() ) );
    return a.exec();
}
