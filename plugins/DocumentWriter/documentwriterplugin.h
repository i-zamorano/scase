#ifndef DOCUMENTWRITERPLUGIN_H
#define DOCUMENTWRITERPLUGIN_H

#include <QObject>
#include <QString>
#include <QList>
#include <QRegExp>
#include <QSettings>

#include "scase1_plugin_documentwriter_global.h"

#include "interfaces/IPlugin.h"
#include "interfaces/IBrowserLevel.h"
#include "interfaces/IBrowserItem.h"

#include "dwptextedit.h"

class SCASE1_PLUGIN_DOCUMENTWRITERSHARED_EXPORT DocumentWriterPlugin : public QObject, public IPlugin
{
    Q_OBJECT
    Q_INTERFACES(IPlugin)

public:
    DocumentWriterPlugin();

    //IPlugin methods
    bool invokeAction(const QString actionName_);
    QString getBrowserTree();
    QWidget *getOutputWidget();
    QString getName();
    QString getPluginPath();
    void setupOutputWidget();

private:
    //DocumentWriterPlugin methods
    Q_INVOKABLE void write(QString value);
    Q_INVOKABLE void write(QString value, QString repetitions);
    Q_INVOKABLE void write_special(QString value);
    Q_INVOKABLE void write_special(QString value, QString repetitions);
    Q_INVOKABLE void delete_previous_character();
    Q_INVOKABLE void delete_previous_word();
    Q_INVOKABLE void delete_all();
    Q_INVOKABLE void undo();
    Q_INVOKABLE void move_cursor(QString direction);
    Q_INVOKABLE void move_cursor(QString direction, QString type);

    void updatePresentationWidget();
    void updateRootLevel();

    void delete_content(QTextCursor::MoveOperation);

    QSettings *settings;

    QRegExp rxBasePredictorValidator;

    DWPTextEdit *presentationWidget;

    IBrowserLevel *rootLevel;

#ifdef SCASE1_PLUGIN_DOCUMENTWRITER_PREDICTION_ENABLED
    int predictedItemsAdded;
#endif

};

#endif // DOCUMENTWRITERPLUGIN_H
