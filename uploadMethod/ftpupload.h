/**
This file (c) by : - Martin Hammerchmidt alias Imote

This file is licensed under a
Creative Commons Attribution-NonCommercial-ShareAlike 4.0 International License.

You should have received a copy of the license along with this
work. If not, see <http://creativecommons.org/licenses/by-nc-sa/4.0/>.

If you have contributed to this file, add your name to authors list.
*/

#ifndef FTPUPLOAD_H
#define FTPUPLOAD_H

#include <SFML/Network.hpp>

#include <string>
#include <memory>
#include <iostream>
#include <QThread>
#include <QString>
#include "shared.h"

class FileManager;

class FTPUpload : public QThread
{
    Q_OBJECT
public:
    ~FTPUpload();
    FTPUpload(FileManager * parent);

    void setHost(QString const &host);
    void setPort(unsigned int const port);
    void setUsername(QString const &username);
    void setPassword(QString const &password);
    void setBasepath(QString const &basePath);
    void insertFile(File const &file);

    void autoSendFile(); //This function manage automatically all the process to send file

    Uplimg::FTPStatus status;

    void run() Q_DECL_OVERRIDE //Start the sending process with collected informations
    {
        this->autoSendFile();
        this->exec();
    }

protected:
    bool openConnexion();
    bool closeConnexion();
    bool sendFile();

    std::unique_ptr<sf::Ftp> ftpClient;

    QString host;
    unsigned int port;
    QString username;
    QString password;
    QString basePath;
    File file;

    FileManager * parent;
    bool isOpenedConnection;

signals:
    void operationFinished();

};

#endif // FTPUPLOAD_H
