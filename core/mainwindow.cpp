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

#include "mainwindow.h"

#include <QDebug>
#include <QDir>
#include <QPluginLoader>
#include <QCoreApplication>
#include <QVBoxLayout>
#include <QKeyEvent>
#include <QStack>
#include <QRect>
#include <QApplication>
#include <QDesktopWidget>
#include <QPoint>
#include <QtCore/qmath.h>
#include <QTextStream>
#include <QDateTime>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    setup();
}

MainWindow::~MainWindow()
{
    hideCurrentPlugin();
    closeActionLogger();
}

void MainWindow::keyPressEvent(QKeyEvent *event) {
    if (event->key() == Qt::Key_Tab) {
        browser->executeItem();
    } else {
        QMainWindow::keyPressEvent(event);
    }
}

void MainWindow::setup() {
    setUnifiedTitleAndToolBarOnMac(true);
    setWindowTitle("SCASE1");

    int screenCount = QApplication::desktop()->screenCount();
    int screenIndex = (screenCount > 1) ? 1 : 0;
    screenResolution = QApplication::desktop()->screenGeometry(screenIndex);

#ifdef SCASE1_DEBUG_LEVEL_VERBOSE
    qDebug() << "MainWindow::setup:screenCount?" << screenCount;
    qDebug() << "MainWindow::setup:screenIndex?" << screenIndex;
    qDebug() << "MainWindow::setup:screenResolution.x()?" << screenResolution.topLeft().x();
    qDebug() << "MainWindow::setup:screenResolution.y()?" << screenResolution.topLeft().y();
#endif
    move(screenResolution.topLeft());

#ifdef SCASE1_FULLSCREEN
#ifdef SCASE1_DEBUG_LEVEL_VERBOSE
    qDebug() << "MainWindow::setup:windowState before call to showFullScreen?" << windowState();
#endif
    resize(screenResolution.width(), screenResolution.height());
    setWindowState(windowState() | Qt::WindowFullScreen);
#ifdef SCASE1_DEBUG_LEVEL_VERBOSE
    qDebug() << "MainWindow::setup:windowState after call to showFullScreen?" << windowState();
#endif
#else
    resize(1024, 768);
#endif

    QString dataDirPath = QString("data/");

#ifdef SCASE1_PLUGIN_DEBUG_LEVEL_VERBOSE
    qDebug() << "MainWindow::setup:dataDirPath?" << dataDirPath();
#endif

    settings = new QSettings(dataDirPath + "config.ini", QSettings::IniFormat, this);

    bell = new QSound(dataDirPath + "bell.wav", this);
    click = new QSound(dataDirPath + "click.wav", this);

    browser = new Browser(this);

    isActionLogActive = settings->value("general/log_actions", true).toBool();

#ifdef SCASE1_PLUGIN_DEBUG_LEVEL_VERBOSE
    qDebug() << "MainWindow::setup:isActionLogActive?" << (isActionLogActive ? "yes":"no");
#endif

    openActionLogger();

    setupInterface();
    setupPlugins();

    connect(browser, SIGNAL(ringBellSignal()), this, SLOT(ringBell()));

    browser->setPauseDelay(settings->value("zone_browser/pause_after_action", "1000").toInt());
    browser->start(zoneBrowser, settings->value("zone_browser/item_presentation_time", "1000").toInt());

    raise();
}

void MainWindow::openActionLogger() {
    if (isActionLogActive) {
#ifdef SCASE1_PLUGIN_DEBUG_LEVEL_VERBOSE
    qDebug() << "MainWindow::openActionLogger:filename = " << filepath;
#endif
        QDir dir(QDir::currentPath());
        dir.mkpath("user/log");

        QDateTime currentDateTime = QDateTime::currentDateTime();

        QString logFilename = QDir::toNativeSeparators(QString("user/log/%2.log").arg(currentDateTime.toString("yyyyMMdd_HHmmss")));

        actionLog = new QFile(logFilename);

        if (!actionLog->open(QIODevice::WriteOnly | QIODevice::Text)) {
#ifdef SCASE1_PLUGIN_DEBUG_LEVEL_VERBOSE
            qDebug() << "MainWindow::openActionLogger:could not open action log file";
#endif
            actionLog = NULL;
        }
    }
}

void MainWindow::logAction(QString action, QString message) {
    if (isActionLogActive && (actionLog != NULL)) {
        QDateTime currentDateTime = QDateTime::currentDateTime();

        QString log = QString("%1 - %2 - %3").arg(currentDateTime.toString("yyyy-MM-dd HH:mm:ss"), action, message);
        QTextStream out(actionLog);
        out << log << endl;
        actionLog->flush();
    }
}

void MainWindow::closeActionLogger() {
    if (isActionLogActive && (actionLog != NULL)) {
        actionLog->flush();
        actionLog->close();
    }
}

void MainWindow::setupInterface() {
    zoneFeedback = new QStackedWidget(this);
    zoneBrowser = new BrowserPresentationWidget(this);
    zoneInteraction = new InteractionWidget(this);

    connect(zoneInteraction, SIGNAL(activated()), this, SLOT(clickSound()), Qt::UniqueConnection);
    connect(zoneInteraction, SIGNAL(activated()), browser, SLOT(executeItem()), Qt::UniqueConnection);
    connect(zoneInteraction, SIGNAL(userHasEntered()), browser, SLOT(stopTimer()), Qt::UniqueConnection);
    connect(zoneInteraction, SIGNAL(userHasLeft()), browser, SLOT(startTimer()), Qt::UniqueConnection);

#ifdef SCASE1_DEBUG_LEVEL_VERBOSE
    QStringList keys = settings->allKeys();
    foreach (QString key, keys) {
        qDebug() << "MainWindow::setupInterface:key?" << key.toUtf8();
    }
#endif

    zoneInteraction->setActivationDelay(settings->value("zone_interaction/dwell_time", "500").toInt());
    zoneInteraction->setRefractoryPeriod(settings->value("zone_interaction/refractory_period", "500").toInt());
    int zoneInteractionSize = settings->value("zone_interaction/size", "100").toInt();
    int zoneBrowserSize = settings->value("zone_browser/size", "100").toInt();
    int zoneInteractionSeparationSize = settings->value("zone_interaction/separation", "100").toInt();

    zoneFeedback->setStyleSheet(QString("QWidget { background-color:#ffffff; }"));

    zoneInteraction->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    zoneBrowser->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    zoneFeedback->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    int w, h, x, y;

    w = screenResolution.width();
    h = zoneInteractionSize;
    x = screenResolution.topLeft().x();
    y = screenResolution.topLeft().y();

    zoneInteraction->setGeometry(x, y, w, h);

    w = screenResolution.width();
    h = screenResolution.height() - zoneInteraction->height() - zoneInteractionSeparationSize;
    x = screenResolution.topLeft().x();
    y = screenResolution.topLeft().y() + zoneInteraction->height() + zoneInteractionSeparationSize;

    zoneFeedback->setGeometry(x, y, w, h);

    w = screenResolution.width() - 100;
    h = zoneBrowserSize;
    x = screenResolution.topLeft().x() + ((screenResolution.width() - w) / 2);
    y = screenResolution.topLeft().y() + (((screenResolution.height() - h) / 6) * 5);

    zoneBrowser->setGeometry(x, y, w, h);

    zoneBrowser->setupStyle(settings->value("zone_browser/color", "000000").toString(), settings->value("zone_browser/background_color", "ffffff").toString(), settings->value("zone_browser/background_color_special", "aaaaff").toString());
    zoneBrowser->setup();
    zoneBrowser->setSpeechRate(settings->value("zone_browser/speech_rate", "0.0").toDouble());

    zoneBrowser->show();
    zoneBrowser->raise();
}

void MainWindow::setupPlugins() {
    pluginHandler = new PluginHandler(this);

    connect(browser, SIGNAL(executeActionFromPlugin(QString,QString)), this, SLOT(logAction(QString,QString)));
    connect(browser, SIGNAL(executeActionFromPlugin(QString,QString)), pluginHandler, SLOT(invokeActionFromPlugin(QString,QString)));
    connect(browser, SIGNAL(pluginSelected(IPlugin*)), this, SLOT(pluginSelected(IPlugin*)));

    QString pluginsDirPath = QString("plugins");

#ifdef SCASE1_DEBUG_LEVEL_VERBOSE
    qDebug() << "pluginsDirPath:" << pluginsDirPath;
  #endif

    QDir pluginsDir = QDir(pluginsDirPath);

    foreach (QString fileName, pluginsDir.entryList(QDir::Files)) {
        QString pluginNativePath = QDir::toNativeSeparators(pluginsDir.absoluteFilePath(fileName));
        QPluginLoader loader(pluginNativePath);
        QObject *plugin = loader.instance();
        if (plugin) {
            QString pluginName = IPLUGIN(plugin)->getName();
            IPLUGIN(plugin)->setUserPath("user");
#ifdef SCASE1_DEBUG_LEVEL_VERBOSE
            qDebug() << "Loaded plugin" << pluginName << "from" << fileName;
#endif
            pluginHandler->registerPlugin(pluginName, IPLUGIN(plugin), browser, zoneFeedback);
        }
    }

    currentPlugin = NULL;
}

void MainWindow::pluginSelected(IPlugin *plugin) {
    hideCurrentPlugin();

    if (plugin != NULL) {
        plugin->show(zoneFeedback);
    }

    currentPlugin = plugin;
}

void MainWindow::hideCurrentPlugin() {
    if (currentPlugin != NULL) {
        currentPlugin->hide();
    }
}

void MainWindow::ringBell() {
#ifdef SCASE1_DEBUG_LEVEL_VERBOSE
    qDebug() << "MainWindow::ringBell:bell->fileName" << bell->fileName();
#endif
    bell->stop();
    bell->play();
}

void MainWindow::clickSound() {
#ifdef SCASE1_DEBUG_LEVEL_VERBOSE
    qDebug() << "MainWindow::clickSound:click->fileName" << click->fileName();
#endif
    click->stop();
    click->play();
}
