#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QWidget>
#include <QLabel>
#include <QStackedWidget>
#include <QSound>
#include <QResizeEvent>
#include <QSettings>


#include "interfaces/IPlugin.h"

#include "pluginhandler.h"

#include "browser.h"
#include "browserpresentationwidget.h"

#include "interactionwidget.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();

public slots:
    void pluginSelected(IPlugin *plugin);
    void ringBell();

private:
    void keyPressEvent(QKeyEvent *event);

    void setup();
    void setupInterface();
    void setupPlugins();

    PluginHandler *pluginHandler;
    Browser *browser;

    QStackedWidget *zoneFeedback;
    InteractionWidget *zoneInteraction;
    BrowserPresentationWidget *zoneBrowser;

    QSound *bell;

    QSettings *settings;
};

#endif // MAINWINDOW_H
