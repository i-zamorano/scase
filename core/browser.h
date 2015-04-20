#ifndef BROWSER_H
#define BROWSER_H

#include <QObject>
#include <QString>
#include <QDomElement>
#include <QLabel>
#include <QStack>
#include <QTimer>

#include "interfaces/IBrowserDelegate.h"
#include "interfaces/IPlugin.h"

#include "browserlevel.h"
#include "browserpresentationwidget.h"

#define SCASE1_MODULE_BROWSER "net.adapar.SCASE1.Browser/1.0"

enum BROWSER_NAVIGATION_STATUS {
    BROWSER_READ_FROM_TREE,
    BROWSER_PSEUDOITEM_UP_ONE_LEVEL
};

class Browser : public QObject, public IBrowserDelegate
{
    Q_OBJECT
    Q_INTERFACES(IBrowserDelegate)

public:
    explicit Browser(QObject *parent = 0);

    void setup();
    void setupPluginTree(IPlugin *plugin);
    void start(BrowserPresentationWidget *presentationDelegate_, int delay);

    virtual void addItemToLevel(IBrowserLevel *level, int pos, QString name, QString moduleName, QString actionName, bool isSpecial);

    void test();

signals:
    void nextItemSelected(bool isSpecial);
    void pluginSelected(IPlugin *plugin);
    void executeActionFromPlugin(QString moduleName, QString actionName);
    void ringBellSignal();

public slots:
    void nextItem();
    void executeItem();
    void goToPreviousStop();
    void goToLevel(IBrowserLevel *level);
    void stopTimer();
    void startTimer();
    void ring_bell();

private:
    BROWSER_NAVIGATION_STATUS navigationStatus;

    BrowserLevel *rootLevel;

    BrowserItem *readDeploymentTree(const QString pluginName, const QString deploymentTree);
    BrowserItem *readDeploymentItem(const QString pluginName, QDomElement currentElement);

    BrowserLevel *currentLevel;
    BrowserItem *currentItem;

    QStack <BrowserLevel *> levelStack;
    BrowserPresentationWidget *presentationDelegate;

    int itemDelay;
    QTimer *timer;

    void setCurrentLevel(BrowserLevel *level);
    void updatePresentationDelegate();

    void goToPreviousLevel(bool waitForStop = false);
};

#endif // BROWSER_H
