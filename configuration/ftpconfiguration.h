/**
This file (c) by : - Martin Hammerchmidt alias Imote

This file is licensed under a
Creative Commons Attribution-NonCommercial-ShareAlike 4.0 International License.

You should have received a copy of the license along with this
work. If not, see <http://creativecommons.org/licenses/by-nc-sa/4.0/>.

If you have contributed to this file, add your name to authors list.
*/

#ifndef FTPCONFIGURATION_H
#define FTPCONFIGURATION_H

#include <QWidget>
#include <QString>
#include <QVBoxLayout>
#include <QFormLayout>
#include <QLineEdit>
#include <QLabel>
#include <QSpinBox>
#include <QSettings>
#include <QHBoxLayout>
#include <QPushButton>
#include <QGroupBox>

#include "shared.h"

class FTPConfiguration : public QWidget
{
    Q_OBJECT
public:
    explicit FTPConfiguration(QWidget *parent = 0);

protected:
    QSettings settings;

    //Main layout and main form
    QVBoxLayout * layout;
    QGroupBox * mainGroupLayout;
    QFormLayout * formLayout;

    QLineEdit * host;
    QSpinBox * port;
    QLineEdit * username;
    QLineEdit * password;
    QLineEdit * basePath;
    QLineEdit * webPath;

    QHBoxLayout * validateLayout;
    ButtonBlue * validate;

    void setUpUI();

public slots :
    void hostSettingModified(QString);
    void portSettingModified(int);
    void usernameSettingModified(QString);
    void passwordSettingModified(QString);
    void basePathSettingModified(QString);
    void webPathSettingModified(QString);

};

#endif // FTPCONFIGURATION_H
