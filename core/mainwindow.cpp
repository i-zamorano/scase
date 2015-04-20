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

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    setup();
}

MainWindow::~MainWindow()
{
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
    QRect screenres = QApplication::desktop()->screenGeometry(screenIndex);

#ifdef SCASE1_DEBUG_LEVEL_VERBOSE
    qDebug() << "MainWindow::setup:screenCount?" << screenCount;
    qDebug() << "MainWindow::setup:screenIndex?" << screenIndex;
    qDebug() << "MainWindow::setup:screenres.x()?" << screenres.topLeft().x();
    qDebug() << "MainWindow::setup:screenres.y()?" << screenres.topLeft().y();
#endif
    move(screenres.topLeft());

#ifdef SCASE1_FULLSCREEN
    resize(screenres.width(), screenres.height());
    showFullScreen();
#else
    resize(1024, 768);
#endif

    QString dataDirPath = QCoreApplication::applicationDirPath() + QString("/data/");

#ifdef SCASE1_PLUGIN_DEBUG_LEVEL_VERBOSE
    qDebug() << "MainWindow::setup:dataDirPath?" << getPluginPath();
#endif

    settings = new QSettings(dataDirPath + "config.ini", QSettings::IniFormat, this);

    bell = new QSound(dataDirPath + "bell.wav", this);

    browser = new Browser(this);

    setupInterface();
    setupPlugins();

    connect(browser, SIGNAL(ringBellSignal()), this, SLOT(ringBell()));

    browser->start(zoneBrowser, settings->value("zone_browser/item_presentation_time", "1000").toInt());

    raise();
}

void MainWindow::setupInterface() {
    zoneFeedback = new QStackedWidget(this);
    zoneInteraction = new InteractionWidget(this);
    zoneBrowser = new BrowserPresentationWidget(this);

    connect(zoneInteraction, SIGNAL(activated()), browser, SLOT(executeItem()));
    connect(zoneInteraction, SIGNAL(userHasEntered()), browser, SLOT(stopTimer()));
    connect(zoneInteraction, SIGNAL(userHasLeft()), browser, SLOT(startTimer()));

    QString configuredBackgroundColor = settings->value("general/background_color", "777777").toString().trimmed();

#ifdef SCASE1_DEBUG_LEVEL_VERBOSE
    QStringList keys = settings->allKeys();
    foreach (QString key, keys) {
        qDebug() << "MainWindow::setupInterface:key?" << key.toUtf8();
    }
#endif

    zoneInteraction->setActivationDelay(settings->value("zone_interaction/dwell_time", "500").toInt());

    int zoneInteractionSize = settings->value("zone_interaction/size", "100").toInt();
    int zoneBrowserSize = settings->value("zone_browser/size", "100").toInt();
    int zoneInteractionSeparation = settings->value("zone_interaction/separation", 50).toInt();
    int zoneBrowserSeparation = settings->value("zone_browser/separation", 50).toInt();

    if ((zoneInteractionSize <= 0) || (zoneInteractionSize > size().height())) {
        zoneInteractionSize = qCeil(size().height() * 0.2);
    }

    if ((zoneBrowserSize <= 0) || (zoneBrowserSize > size().height())) {
        zoneBrowserSize = qCeil(size().height() * 0.2);
    }

    if ((zoneInteractionSeparation <= 0) || (zoneInteractionSeparation > zoneInteractionSize)) {
        zoneInteractionSeparation = zoneInteractionSize;
    }

    if ((zoneBrowserSeparation <= 0) || (zoneBrowserSeparation > zoneBrowserSize)) {
        zoneBrowserSeparation = zoneBrowserSize;
    }

#ifdef SCASE1_DEBUG_LEVEL_VERBOSE
    qDebug() << "MainWindow::setupInterface:this->size().height()?" << size().height();
    qDebug() << "MainWindow::setupInterface:configuredBackgroundColor?" << configuredBackgroundColor;
    qDebug() << "MainWindow::setupInterface:zoneInteractionSize?" << zoneInteractionSeparation;
    qDebug() << "MainWindow::setupInterface:zoneBrowserSize?" << zoneBrowserSeparation;
    qDebug() << "MainWindow::setupInterface:zoneInteractionSeparation?" << zoneInteractionSeparation;
    qDebug() << "MainWindow::setupInterface:zoneBrowserSeparation?" << zoneBrowserSeparation;
    qDebug() << "MainWindow::setupInterface:settings(zone_interaction/ubication)?" << settings->value("zone_interaction/ubication", "NONE").toString();
#endif

    zoneInteraction->setMinimumHeight(zoneInteractionSize);
    zoneBrowser->setMinimumHeight(zoneBrowserSize);
    zoneFeedback->setStyleSheet(QString("QWidget { background-color:#%1}").arg(settings->value("general/background_color", "777777").toString().trimmed()));

    zoneInteraction->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    zoneBrowser->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Fixed);
    zoneFeedback->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    zoneBrowser->setupStyle(settings->value("zone_browser/color", "000000").toString(), settings->value("zone_browser/background_color", "ffffff").toString(), settings->value("zone_browser/background_color_special", "aaaaff").toString(), zoneBrowserSize);
    zoneBrowser->setup();

    QVBoxLayout *layout = new QVBoxLayout();
    layout->setContentsMargins(0, 0, 0, 0);

    if (settings->value("zone_interaction/ubication", "top").toString() == "top") {
        layout->setSpacing(0);
        layout->addWidget(zoneInteraction);
        layout->addSpacing(zoneInteractionSeparation);
        layout->addWidget(zoneBrowser);
        layout->addSpacing(zoneBrowserSeparation);
        layout->addWidget(zoneFeedback);
    } else {
        layout->setSpacing(0);
        layout->addWidget(zoneFeedback);
        layout->addSpacing(zoneBrowserSeparation);
        layout->addWidget(zoneBrowser);
        layout->addSpacing(zoneInteractionSeparation);
        layout->addWidget(zoneInteraction);
    }

    QWidget *centralWidget = new QWidget(this);
    centralWidget->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    centralWidget->setStyleSheet(QString("QWidget { background-color: #%1 }").arg(settings->value("general/background_color", "777777").toString().trimmed()));
    centralWidget->setLayout(layout);

    setCentralWidget(centralWidget);
}

void MainWindow::setupPlugins() {
    pluginHandler = new PluginHandler(this);

    connect(browser, SIGNAL(executeActionFromPlugin(QString,QString)), pluginHandler, SLOT(invokeActionFromPlugin(QString,QString)));
    connect(browser, SIGNAL(pluginSelected(IPlugin*)), this, SLOT(pluginSelected(IPlugin*)));

    QString pluginsDirPath = QCoreApplication::applicationDirPath() + QString("/plugins");

#ifdef SCASE1_DEBUG_LEVEL_VERBOSE
    qDebug() << "pluginsDirPath:" << pluginsDirPath;
  #endif

    QDir pluginsDir = QDir(pluginsDirPath);

    foreach (QString fileName, pluginsDir.entryList(QDir::Files)) {
        QPluginLoader loader(pluginsDir.absoluteFilePath(fileName));
        QObject *plugin = loader.instance();
        if (plugin) {
            QString pluginName = IPLUGIN(plugin)->getName();
#ifdef SCASE1_DEBUG_LEVEL_VERBOSE
            qDebug() << "Loaded plugin" << pluginName << "from" << fileName;
#endif
            pluginHandler->registerPlugin(pluginName, IPLUGIN(plugin), browser, zoneFeedback);
        }
    }
}

void MainWindow::pluginSelected(IPlugin *plugin) {
    if (plugin != NULL) {
        QWidget *outputWidget = plugin->getOutputWidget();
        if (outputWidget != NULL) {
            zoneFeedback->setCurrentWidget(outputWidget);
            outputWidget->show();
            outputWidget->raise();
            plugin->setupOutputWidget();
        }
    }
}

void MainWindow::ringBell() {
#ifdef SCASE1_DEBUG_LEVEL_VERBOSE
    qDebug() << "MainWindow::ringBell:bell->isAvailable?" << (bell->isAvailable() ? "yes" : "no");
    qDebug() << "MainWindow::ringBell:bell->fileName" << bell->fileName();
#endif
    if (bell->isAvailable()) {
        bell->stop();
        bell->play();
    }
}
