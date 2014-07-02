#include "systemtrayicon.h"


SystemTrayIcon::SystemTrayIcon(QObject *qobject) :
    QSystemTrayIcon(qobject),
    FTPWebPathSettingName("configuration/ftp/webPath"),
    HTTPWebPathSettingName("configuration/http/webPath"),
    choosedMethodSettingName("configuration/method"),
    applicationName("Uplimg")
{
    configurationWindows = new ConfigurationWindows;
    screenManager = new ScreenManager(this);
    setIcon(QIcon(":/small.png"));
    setToolTip(tr("Daemon is running and waiting"));

    base = new QWidget;

    setUpContextMenu();

    QSystemTrayIcon::show();

    QObject::connect(takeScreen, SIGNAL(triggered()), this, SLOT(takeFullScrenTriggered()));
    QObject::connect(takeSelectedScreen, SIGNAL(triggered()), this, SLOT(takeSelectedAreaScreenTriggered()));
    QObject::connect(uploadFile, SIGNAL(triggered()), this, SLOT(uploadSelectedFileTriggered()));
    QObject::connect(uploadClipboard, SIGNAL(triggered()), this, SLOT(uploadClipboardTriggered()));
    QObject::connect(showConfiguration, SIGNAL(triggered()), this, SLOT(showWindowConfigurationTriggered()));
    QObject::connect(quit, SIGNAL(triggered()), qApp, SLOT(quit()));
}

void SystemTrayIcon::setUpContextMenu()
{
    systemTrayMenu = new QMenu;

    takeScreen = systemTrayMenu->addAction(tr("Take new screen"));
    takeSelectedScreen = systemTrayMenu->addAction(tr("Take new selected screen"));
    uploadFile = systemTrayMenu->addAction(tr("Upload specific file"));
    uploadClipboard = systemTrayMenu->addAction(tr("Upload clipboard"));
    systemTrayMenu->addSeparator();
    showConfiguration = systemTrayMenu->addAction(tr("Configuration...", "In system tray icon"));
    quit = systemTrayMenu->addAction(tr("Exit !"));

    setContextMenu(systemTrayMenu);
}

void SystemTrayIcon::takeSelectedAreaScreenTriggered()
{
    std::cerr << "Started screen process\n";

    fileName = getNewFileName(".png");
    pathToFile = getFileTempPath(fileName);

    std::cerr << "Names generated\n";
    screenManager->captureSelectedZone(pathToFile);
}

void SystemTrayIcon::sendSelectedArea()
{
    if (screenManager->autoSendFile(pathToFile))
        {

            std::cerr << "File sended\n";

            const QString urlPath = getUploadedFileURL(fileName);
            QApplication::clipboard()->setText(urlPath);

            this->showMessage(applicationName, tr("Congratulation !\nUpload success. The URL is :\n") + urlPath);
            std::cerr << "Upload success !\n";
        }
    else
        throwErrorAlert(Uplimg::ErrorList::UPLOAD_FAIL);
}

void SystemTrayIcon::takeFullScrenTriggered()
{
    std::cerr << "Started screen process\n";

    QString fileName = getNewFileName(".png");
    QString pathToFile = getFileTempPath(fileName);

    if (screenManager->autoSendFile(screenManager->captureFullScreen(pathToFile)))
        fileSended(fileName);
}

void SystemTrayIcon::fileSended(QString fileName)
{
    const QString urlPath = getUploadedFileURL(fileName);
    QApplication::clipboard()->setText(urlPath);

    this->showMessage(applicationName, tr("Congratulation !\nUpload success. The URL is :\n") + urlPath);
}

void SystemTrayIcon::uploadSelectedFileTriggered()
{
    QString path = FileDialog::getOpenFileName(base, tr("Select file"));

    if (screenManager->autoSendFile(path))
        {
            this->showMessage(applicationName, tr("Congratulation !\nFile uploaded !"));
        }
}

void SystemTrayIcon::uploadClipboardTriggered()
{
    const QString fileName = getNewFileName(".txt");
    const QString filePath = getFileTempPath(fileName);
    std::ofstream file(filePath.toStdString().c_str());

    if (file)
        {
            file << QApplication::clipboard()->text().toStdString();
            file.close();
            if (screenManager->autoSendFile(filePath))
                {
                    const QString url = getUploadedFileURL(fileName);
                    this->showMessage(applicationName, "Clipboard upload success !\n" + url);
                    QApplication::clipboard()->setText(url);
                }
        }
}

QString SystemTrayIcon::getNewFileName(QString ending)
{
    QTime time = QTime::currentTime();
    QDate date = QDate::currentDate();

    return QString::number(date.dayOfYear())
           + QString::number(time.hour())
           + QString::number(time.minute())
           + QString::number(time.second())
           + ending;
}

QString SystemTrayIcon::getFileTempPath(const QString &screenName)
{
    return QStandardPaths::writableLocation(QStandardPaths::TempLocation)
           + "/"
           + screenName;
}

QString SystemTrayIcon::getUploadedFileURL(const QString &fileName)
{
    if(getUploadMethod() == Uplimg::UploadMethod::FTP)
        return settings.value(FTPWebPathSettingName, "http://").toString() + fileName;
    else if(getUploadMethod() == Uplimg::UploadMethod::HTTP)
        return settings.value(HTTPWebPathSettingName, "http://").toString() + fileName;
    else
        return "error";
}

void SystemTrayIcon::showWindowConfigurationTriggered()
{
    configurationWindows->show();
}

void SystemTrayIcon::throwErrorAlert(const QString &text)
{
    QMessageBox::critical(0, "Uplimg", text);
}

void SystemTrayIcon::throwErrorAlert(const Uplimg::ErrorList &error)
{
    if (error == Uplimg::ErrorList::UPLOAD_FAIL)
        {
            const QString text(tr("Upload failed.\nYou must verify Uplimg's configuration or your Internet configuration to solve the problem."));
            QMessageBox::critical(0, "Uplimg", text);
        }
    else if(error == Uplimg::ErrorList::UPLOAD_METHOD_NOT_CHOOSED)
        {
            const QString text(tr("We can't upload anything.\nYou must configure method to upload before."));
            QMessageBox::critical(0, "Uplimg", text);
        }
}

Uplimg::UploadMethod SystemTrayIcon::getUploadMethod() const
{
    if (settings.value(choosedMethodSettingName).toString().toStdString() == "FTP")
        return Uplimg::UploadMethod::FTP;
    else if (settings.value(choosedMethodSettingName).toString().toStdString() == "HTTP")
        return Uplimg::UploadMethod::HTTP;
    else
        return Uplimg::UploadMethod::ERROR;
}
