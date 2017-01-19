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

#ifndef DOCUMENTWRITERPLUGIN_H
#define DOCUMENTWRITERPLUGIN_H

#include <QObject>
#include <QString>
#include <QList>
#include <QRegExp>
#include <QSettings>
#include <QtPlugin>

#include "scase1_plugin_documentwriter_global.h"

#include "interfaces/IPlugin.h"
#include "interfaces/IBrowserLevel.h"
#include "interfaces/IBrowserItem.h"

#include "dwptextedit.h"

#ifdef SCASE1_PLUGIN_DOCUMENTWRITER_PREDICTION_ENABLED
#include "dwppresagecallback.h"
#endif

class Q_DECL_EXPORT DocumentWriterPlugin : public QObject, public IPlugin
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID SCASE1_PLUGIN_DOCUMENTWRITER_NAME)
    Q_INTERFACES(IPlugin)

public:
    DocumentWriterPlugin();

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
    void textHasChanged();

private:
    //IPlugin methods
    void invokeMethodPrivate(const QString actionName_);
    void invokeServicePrivate(const QString serviceName_, const QString command_, QVariant payload);
    void setBrowserItemDelegatePrivate(IBrowserItem *delegate);

    //DocumentWriterPlugin methods
    Q_INVOKABLE void write_prediction(QString value);
    Q_INVOKABLE void write(QString value, QString repetitions = "1", bool isPrediction = false);
    Q_INVOKABLE void write_special(QString value, QString repetitions = "1");
    Q_INVOKABLE void delete_previous_character();
    Q_INVOKABLE void delete_previous_word();
    Q_INVOKABLE void delete_all();
    Q_INVOKABLE void undo();
    Q_INVOKABLE void move_cursor(QString direction);
    Q_INVOKABLE void move_cursor(QString direction, QString type);

    Q_INVOKABLE void service_set_content(QVariant payload);

    void updatePresentationWidget();

    void delete_content(QTextCursor::MoveOperation);

    void setDocumentPath(QString configuredPath);

    QString getRecentCacheFilename();

    void saveRecentCache();
    void saveCurrentVersion();
    void saveContentsTo(QString filepath);

    bool isContextValidForPrediction(std::string context);

    QString getRecentCache();
    QString getContentsFrom(QString filepath);

    QSettings *settings;

    DWPTextEdit *presentationWidget;

    IBrowserLevel *rootLevel;

    bool autosave;

    QString documentPath;

#ifdef SCASE1_PLUGIN_DOCUMENTWRITER_PREDICTION_ENABLED
    int predictedItemsAdded;
    std::string presageStdContext;
    DWPPresageCallback presageCallback;
    Presage presage;
#endif

};

#endif // DOCUMENTWRITERPLUGIN_H
