#ifndef SYSTEMTRAYICON_H
#define SYSTEMTRAYICON_H



#include <QApplication>
#include <QMenu>
#include <QAction>
#include <QList>
#include <QSystemTrayIcon>
#include <QClipboard>
#include <QIcon>
#include <QShortcut>
#include <QFileDialog>
#include <QString>
#include <QMessageBox>

#include <fstream>

#include "configurationwindows.h"
#include "screenmanager.h"
#include "FileDialog.h"
#include "shared.h"

class SystemTrayIcon : public QSystemTrayIcon
{
    Q_OBJECT
public:
    explicit SystemTrayIcon(QObject *qobject = 0);
    Uplimg::UploadMethod getUploadMethod() const;

protected:
    const QString applicationName;
    QSettings settings;

    const QString HTTPWebPathSettingName;
    const QString FTPWebPathSettingName;
    const QString choosedMethodSettingName;

    ConfigurationWindows * configurationWindows;
    ScreenManager * screenManager;

    QMenu * systemTrayMenu;
    QAction * takeScreen;
    QAction * takeSelectedScreen;
    QAction * uploadFile;
    QAction * uploadClipboard;
    QAction * showConfiguration;
    QAction * quit;

    QShortcut * shortcut;

    QWidget * base;

    void setUpContextMenu();
    QString getNewFileName(QString ending);
    QString getFileTempPath(const QString &screenName);
    QString getUploadedFileURL(const QString &fileName);

    //Temp
    QString fileName;
    QString pathToFile;

signals:

public slots :
    void showWindowConfigurationTriggered();
    void takeFullScrenTriggered();
    void takeSelectedAreaScreenTriggered();
    void uploadSelectedFileTriggered();
    void uploadClipboardTriggered();
    void fileSended(QString fileName);

    void sendSelectedArea();
    void throwErrorAlert(const QString &text);
    void throwErrorAlert(const Uplimg::ErrorList &error);

};

#endif // SYSTEMTRAYICON_H