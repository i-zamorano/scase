/*************

    SCASE: extendable prototype of an assistive communication tool for people with Locked-in Syndrome

    Copyright (C) 2014  Andrés Aparicio

    This file is part of SCASE.

    SCASE is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    SCASE is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with SCASE.  If not, see <http://www.gnu.org/licenses/>.

    **********/

#ifndef FILEMANAGERPLUGIN_H
#define FILEMANAGERPLUGIN_H

#include <QObject>
#include <QString>
#include <QList>
#include <QRegExp>
#include <QSettings>
#include <QtPlugin>
#include <QTextEdit>
#include <QTextToSpeech>

#include "scase1_plugin_filemanager_global.h"

#include "interfaces/IPlugin.h"
#include "interfaces/IBrowserLevel.h"
#include "interfaces/IBrowserItem.h"

class Q_DECL_EXPORT FileManagerPlugin : public QObject, public IPlugin
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID SCASE1_PLUGIN_FILEMANAGER_NAME)
    Q_INTERFACES(IPlugin)

public:
    FileManagerPlugin();

    //IPlugin methods
    QString getBrowserTree();
    QWidget *getOutputWidget();
    QString getName();
    QString getPluginPath();
    void setupOutputWidget();
    void show(QStackedWidget *container);
    void hide();

signals:
    void requestService(const QString serviceName, const QString command, QVariant payload);
    void requestTransition(const QString serviceName);

protected slots:

private:
    //IPlugin methods
    void invokeMethodPrivate(const QString actionName_);
    void invokeServicePrivate(const QString serviceName_, const QString command_, QVariant payload);
    void setBrowserItemDelegatePrivate(IBrowserItem *delegate);

    //FileManagerPlugin methods
    Q_INVOKABLE void browse_files();
    Q_INVOKABLE void load(QString filename);
    Q_INVOKABLE void new_file();
    Q_INVOKABLE void show_recent_cache();
    Q_INVOKABLE void stop_audio();
    Q_INVOKABLE void read_recent_cache();
    Q_INVOKABLE void clear_presentation_widget();
    Q_INVOKABLE void clear_created_files();
    Q_INVOKABLE void write(QString value, QString repetitions = "1");

    void updatePresentationWidget();

    void setDocumentPath(QString configuredPath);

    QString getSavedDocumentsPath();
    QString getRecentCacheFilename();

    void saveRecentCache();
    void saveCurrentVersion();
    void saveContentsTo(QString filepath);

    QString getRecentCache();
    QString getContentsFrom(QString filepath);

    QSettings *settings;

    QTextEdit *presentationWidget;

    IBrowserLevel *rootLevel;
    IBrowserLevel *filesLevel;
    QTextToSpeech *m_speech;

    QString documentPath;

    int filesAdded;

};

#endif // FILEMANAGERPLUGIN_H
