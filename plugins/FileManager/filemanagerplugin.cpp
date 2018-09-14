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

#include "filemanagerplugin.h"

#include <QDebug>
#include <QMetaMethod>
#include <QStringList>
#include <QFile>
#include <QTextStream>
#include <QDate>
#include <QTime>
#include <QDir>
#include <QCoreApplication>
#include <QGenericArgument>
#include <QFontMetrics>
#include <QTextCursor>
#include <QTextBlockFormat>
#include <QTextDocument>
#include <QStringList>
#include <QTextCodec>
#include <QTextEdit>
#include <QtCore/qmath.h>
#include <QTextCharFormat>
#include <QColor>
#include <QFileInfo>
#include <QGraphicsDropShadowEffect>

#include <locale>

FileManagerPlugin::FileManagerPlugin()
{
    QTextCodec::setCodecForLocale(QTextCodec::codecForName("utf-8"));

    browserItemDelegate = NULL;
    rootLevel = NULL;
    filesLevel = NULL;

    m_speech = new QTextToSpeech(this);

#ifdef SCASE1_PLUGIN_DEBUG_LEVEL_VERBOSE
    qDebug() << "FileManagerPlugin::FileManagerPlugin:pluginDirPath?" << getPluginPath();
#endif

    settings = new QSettings(getPluginPath() + SCASE1_PLUGIN_FILEMANAGER_SETTINGS_FILE + ".ini", QSettings::IniFormat, this);

    presentationWidget = new QTextEdit();
    presentationWidget->setFrameStyle(QFrame::NoFrame);
    presentationWidget->setWindowFlags(Qt::FramelessWindowHint);

    presentationWidget->setWordWrapMode(QTextOption::WrapAtWordBoundaryOrAnywhere);
    presentationWidget->setUndoRedoEnabled(true);
    presentationWidget->ensureCursorVisible();
    presentationWidget->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    filesAdded = 0;
}

void FileManagerPlugin::show(QStackedWidget *container) {
    container->setCurrentWidget(getOutputWidget());
    presentationWidget->show();
    presentationWidget->raise();
    presentationWidget->setFocus();
    setupOutputWidget();
}

void FileManagerPlugin::hide() {
    presentationWidget->hide();
}

void FileManagerPlugin::saveRecentCache() {
#ifdef SCASE1_PLUGIN_DEBUG_LEVEL_VERBOSE
        qDebug() << "FileManagerPlugin::saveRecentCache";
#endif
    saveContentsTo(getRecentCacheFilename());
}

QString FileManagerPlugin::getRecentCacheFilename() {
    return QString("%1recent.txt").arg(documentPath);
}

void FileManagerPlugin::setDocumentPath(QString configuredPath) {
    documentPath = QDir::toNativeSeparators(QString("%1/%2/").arg(userPath, configuredPath));
#ifdef SCASE1_PLUGIN_DEBUG_LEVEL_VERBOSE
    qDebug() << "FileManagerPlugin::setDocumentPath:documentPath?" << documentPath;
#endif
}

QString FileManagerPlugin::getSavedDocumentsPath()
{
    return QDir::toNativeSeparators(QString("%1saved").arg(documentPath));
}

QString FileManagerPlugin::getRecentCache() {
    return getContentsFrom(getRecentCacheFilename());
}

QString FileManagerPlugin::getContentsFrom(QString filepath) {
    QFile inFile(filepath);
    QString content = "";

    if (inFile.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QTextStream in(&inFile);
        content = in.readAll();
#ifdef SCASE1_PLUGIN_DEBUG_LEVEL_VERBOSE
    } else {
        qDebug() << "FileManagerPlugin::getContentsFrom:could not open file for reading";
    }
#else
    }
#endif

    return content;
}

void FileManagerPlugin::saveContentsTo(QString filepath) {
#ifdef SCASE1_PLUGIN_DEBUG_LEVEL_VERBOSE
    qDebug() << "FileManagerPlugin::saveContentsTo:filename = " << filepath;
    qDebug() << "FileManagerPlugin::saveContentsTo:text = " << presentationWidget->toPlainText();
#endif
    QFile outputFile(filepath);

    if (outputFile.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QTextStream out(&outputFile);
        out << presentationWidget->toPlainText();
        outputFile.flush();
        outputFile.close();
#ifdef SCASE1_PLUGIN_DEBUG_LEVEL_VERBOSE
    } else {
        qDebug() << "FileManagerPlugin::saveContentsTo:could not open file for saving";
    }
#else
    }
#endif
}

void FileManagerPlugin::setupOutputWidget() {
    QString configuredColor = settings->value("presentation/color", "000000").toString().trimmed();
    QString configuredBackgroundColor = settings->value("presentation/background_color", "ffffff").toString().trimmed();
    QString configuredSize = settings->value("presentation/size", "100%").toString().trimmed();
    int configuredLines = settings->value("presentation/lines", 5).toInt();

    QString configuredPath = settings->value("storage/document_path", "documents").toString().trimmed();

#ifdef SCASE1_PLUGIN_DEBUG_LEVEL_VERBOSE
    QStringList keys = settings->allKeys();
    foreach (QString key, keys) {
        qDebug() << "FileManagerPlugin::setupOutputWidget:key?" << key.toUtf8();
    }
    qDebug() << "FileManagerPlugin::setupOutputWidget:configuredColor?" << configuredColor;
    qDebug() << "FileManagerPlugin::setupOutputWidget:configuredBackgroundColor?" << configuredBackgroundColor;
    qDebug() << "FileManagerPlugin::setupOutputWidget:configuredSize?" << configuredSize;
    qDebug() << "FileManagerPlugin::setupOutputWidget:configuredLines?" << configuredLines;
    qDebug() << "FileManagerPlugin::setupOutputWidget:configuredPath?" << configuredPath;
#endif

    int containerSize = presentationWidget->parentWidget()->size().height();

    QRegExp rxSizeIsPercentage("^[0-9]+%$");
    QRegExp rxSizeIsNumber("^[0-9]+$");

    bool sizeIsProportional = true;
    int size = 100;

    if ( rxSizeIsPercentage.exactMatch(configuredSize) ) {
        configuredSize.chop(1);
        size = configuredSize.toInt();
        sizeIsProportional = true;
    } else if ( rxSizeIsNumber.exactMatch(configuredSize) ) {
        size = configuredSize.toInt();
        sizeIsProportional = false;
    }

#ifdef SCASE1_PLUGIN_DEBUG_LEVEL_VERBOSE
    qDebug() << "FileManagerPlugin::setupOutputWidget:sizeIsProportional?" << (sizeIsProportional ? "yes" : "no");
    qDebug() << "FileManagerPlugin::setupOutputWidget:containerSize?" << containerSize;
    qDebug() << "FileManagerPlugin::setupOutputWidget:size?" << size;
#endif

    if (sizeIsProportional) {
        size = qCeil((containerSize * size) / 100.0f);
#ifdef SCASE1_PLUGIN_DEBUG_LEVEL_VERBOSE
        qDebug() << "FileManagerPlugin::setupOutputWidget:size(calculated)?" << size;
#endif
    }

    if (size > containerSize) {
        size = containerSize - 300;
    }

    int fontSize = qCeil(size / (configuredLines * 1.5f));

    QString presentationWidgetStyle = QString("QTextEdit { border-width:0px; padding:10px; font-family: Helvetica, Arial; font-size: %1px; background-color: #%2; color: #%3; }").arg(QString::number(fontSize), configuredBackgroundColor, configuredColor);

#ifdef SCASE1_PLUGIN_DEBUG_LEVEL_VERBOSE
    qDebug() << "FileManagerPlugin::setupOutputWidget:fontSize?" << fontSize;
    qDebug() << "FileManagerPlugin::setupOutputWidget:presentationWidgetStyle?" << presentationWidgetStyle;
#endif

    presentationWidget->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    presentationWidget->setStyleSheet(presentationWidgetStyle);
    presentationWidget->setCursorWidth(10);
    presentationWidget->setFixedHeight(size);
    presentationWidget->setReadOnly(true);

    setDocumentPath(configuredPath);

    presentationWidget->setText(getRecentCache());

//    browse_files();
}

QString FileManagerPlugin::getPluginPath() {
    return QString("plugins/");
}

void FileManagerPlugin::invokeMethodPrivate(const QString actionName_) {
    QStringList parts = actionName_.split(",");

    if (parts.size() > 0) {
        QString methodName = QString(parts.at(0));

        QGenericArgument argumentTable[ 10 ];

        parts.removeFirst();

        QStringList signatureArguments;

        int i = 0;

        for (; i < parts.size(); i++) {
            argumentTable[i] = Q_ARG(QString, parts.at(i));
#ifdef SCASE1_PLUGIN_DEBUG_LEVEL_VERBOSE
            qDebug() << "argumentTable[" << i << "] comes from " << parts.at(i);
            qDebug() << "argumentTable[" << i << "] is " << argumentTable[i].name();
#endif
            signatureArguments.append("QString");
        }

        if (i < 10) {
            argumentTable[i] = QGenericArgument(0);
#ifdef SCASE1_PLUGIN_DEBUG_LEVEL_VERBOSE
            qDebug() << "0:argumentTable[" << i << "] is " << argumentTable[i].name();
#endif
            for (i++; i < 10; i++) {
                argumentTable[i] = QGenericArgument();
#ifdef SCASE1_PLUGIN_DEBUG_LEVEL_VERBOSE
                qDebug() << ":argumentTable[" << i << "] is " << argumentTable[i].name();
#endif
            }
        }

        QString signature = methodName + "(" + signatureArguments.join(",") + ")";

#ifdef SCASE1_PLUGIN_DEBUG_LEVEL_VERBOSE
        qDebug() << "signature:" << signature;
#endif

        int methodIndex = metaObject()->indexOfMethod(signature.toLatin1().constData());

        if (methodIndex > -1) {
            QMetaMethod metaMethod = metaObject()->method(methodIndex);
            metaMethod.invoke(this, argumentTable[0], argumentTable[1], argumentTable[2], argumentTable[3], argumentTable[4], argumentTable[5], argumentTable[6], argumentTable[7], argumentTable[8], argumentTable[9]);
#ifdef SCASE1_PLUGIN_DEBUG_LEVEL_VERBOSE
        } else {
            qDebug() << "signature:" << signature << "not found";
#endif
        }
    }
}

void FileManagerPlugin::invokeServicePrivate(const QString serviceName_, const QString command_, QVariant payload)
{
    Q_UNUSED(serviceName_);
    Q_UNUSED(command_);
    Q_UNUSED(payload);
}

QString FileManagerPlugin::getBrowserTree() {
    QString browserTreeFile = getPluginPath() + "lang/" + settings->value("presentation/language", "en").toString().trimmed() + "/" + SCASE1_PLUGIN_FILEMANAGER_BROWSER_TREE_FILE + ".xml";
#ifdef SCASE1_PLUGIN_DEBUG_LEVEL_VERBOSE
    qDebug() << "browserTreeFile:" << browserTreeFile;
#endif

    QFile xmlBrowserTree(browserTreeFile);

    if (!xmlBrowserTree.open(QIODevice::ReadOnly)) {
        return "";
    }

    QTextStream in(&xmlBrowserTree);

    QString output = in.readAll();

    xmlBrowserTree.close();

    return output;
}

QWidget *FileManagerPlugin::getOutputWidget() {
    return presentationWidget;
}

QString FileManagerPlugin::getName() {
    return QString(SCASE1_PLUGIN_FILEMANAGER_NAME);
}

void FileManagerPlugin::setBrowserItemDelegatePrivate(IBrowserItem *delegate) {
    Q_UNUSED(delegate);
    if ((browserItemDelegate != NULL) && browserItemDelegate->hasLevelBelow()) {
        rootLevel = browserItemDelegate->getLevelBelow();
    } else {
        rootLevel = NULL;
    }
}

void FileManagerPlugin::load(QString filename)
{
//    Obtener ruta del archivo antiguo...
    QString path = QString("%1path.txt").arg(documentPath);
    QString filepath = QDir::toNativeSeparators(QString("%1").arg(path));
    QString old_path = "";
    QFile inOldFile(filepath);

    if (inOldFile.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QTextStream in(&inOldFile);
        old_path = in.readAll();
        inOldFile.flush();
        inOldFile.close();
#ifdef SCASE1_PLUGIN_DEBUG_LEVEL_VERBOSE
    } else {
        qDebug() << "FileManagerPlugin::getContentsFrom:could not open file for reading";
    }
#else
    }
#endif

#ifdef SCASE1_PLUGIN_DEBUG_LEVEL_VERBOSE
    qDebug() << "Path:" << old_path;
#endif
//    Actualizar contenido del archivo antiguo...
    presentationWidget->setText(getRecentCache());
    saveContentsTo(old_path);

//    Actualizar nombre del archivo cargado...
    QFile outputFile(filepath);

    if (outputFile.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QTextStream out(&outputFile);
        out << filename;
        outputFile.flush();
        outputFile.close();
    } else {
        qDebug() << "FileManagerPlugin::saveContentsTo:could not open file for saving";
    }

//    Actualizar texto del archivo cargado...
    QFile inFile(filename);
    QString new_content = "";

    if (inFile.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QTextStream in(&inFile);
        new_content = in.readAll();
        inFile.flush();
        inFile.close();
#ifdef SCASE1_PLUGIN_DEBUG_LEVEL_VERBOSE
    } else {
        qDebug() << "FileManagerPlugin::getContentsFrom:could not open file for reading";
    }
#else
    }
#endif

#ifdef SCASE1_PLUGIN_DEBUG_LEVEL_VERBOSE
    qDebug() << "Read:" << new_content;
#endif

    presentationWidget->setText(new_content);

    path = getRecentCacheFilename();
    filepath = QDir::toNativeSeparators(QString("%1").arg(path));

    saveContentsTo(filepath);

    updatePresentationWidget();

    emit requestService("editor", "set_content", QVariant(presentationWidget->toPlainText()));
    emit requestTransition("editor");
}

void FileManagerPlugin::new_file()
{
    QDate currentDate = QDate::currentDate();

    QString path = getSavedDocumentsPath();
    QString filename = QString("%1%2%3-%4.txt").arg(currentDate.toString("yyyy"), currentDate.toString("MM"), currentDate.toString("dd"), QTime::currentTime().toString("HHmmss"));
    QString filepath = QDir::toNativeSeparators(QString("%1/%2").arg(path, filename));

#ifdef SCASE1_PLUGIN_DEBUG_LEVEL_VERBOSE
    qDebug() << "FileManagerPlugin::save:currentPath?" << QDir::currentPath();
    qDebug() << "FileManagerPlugin::save:path?" << path;
    qDebug() << "FileManagerPlugin::save:filename?" << filename;
#endif

    QDir dir(QDir::currentPath());
    dir.mkpath(path);

    presentationWidget->setText("");
    saveContentsTo(filepath);

    load(filepath);

//    emit requestService("editor", "set_content", QVariant(""));

    browserDelegate->goToLevel(rootLevel);
}

void FileManagerPlugin::show_recent_cache()
{
    presentationWidget->setText(getRecentCache());
}

void FileManagerPlugin::stop_audio()
{
    m_speech->stop();
    browserDelegate->startTansitions();
    browserDelegate->goToLevel(rootLevel->getContainer()->getLevelBelow());
}

void FileManagerPlugin::read_recent_cache()
{
    browserDelegate->stopTansitions();
    m_speech->say(getRecentCache());
}

void FileManagerPlugin::clear_presentation_widget()
{
    presentationWidget->setText("");
}

void FileManagerPlugin::clear_created_files()
{
#ifdef SCASE1_PLUGIN_DEBUG_LEVEL_VERBOSE
    qDebug() << filesAdded;
#endif

    if (filesAdded > 0) {
        if (filesLevel != NULL) {
            for (int i = 0; i < filesAdded; i++) {
                filesLevel->getContainer()->getLevelBelow()->removeItemAtPos(0);
            }
        }

        filesAdded = 0;
    }
}

void FileManagerPlugin::browse_files()
{
#ifdef SCASE1_PLUGIN_DEBUG_LEVEL_VERBOSE
    qDebug() << "browse_files:" << getName();
#endif

    rootLevel->setContainer(browserItemDelegate);

    if ( filesLevel != NULL) {
        if (filesLevel->getContainer()->hasLevelBelow()) {
            clear_created_files();
        }
    } else {
        filesLevel = browserDelegate->addLevel();
    }

#ifdef SCASE1_PLUGIN_DEBUG_LEVEL_VERBOSE
    qDebug() << "new_level??:" << filesLevel->getContainer()->hasLevelBelow();
#endif

    QString path = getSavedDocumentsPath();
    QDir dir(path);

    int index = 0;

    foreach (QFileInfo file, dir.entryInfoList()) {
        if (file.fileName() != "" && file.fileName() != "." && file.fileName() != "..") {
            browserDelegate->addItemToLevel(filesLevel, index, file.fileName(), getName(), QString("load,%1").arg(path + "/" + file.fileName()), true);
            index++;
            filesAdded++;
        }
    }
}

void FileManagerPlugin::updatePresentationWidget()
{
#ifdef SCASE1_PLUGIN_DEBUG_LEVEL_VERBOSE
    qDebug() << "updatePresentationWidget: start";
#endif

    qDebug() << documentPath;
}

void FileManagerPlugin::write(QString value, QString repetitions) {
#ifdef SCASE1_PLUGIN_DEBUG_LEVEL_VERBOSE
    qDebug() << "value:" << value;
    qDebug() << "repetitions:" << repetitions;
#endif

    int total = repetitions.toInt();

    if (total > 0) {
        QTextCursor cursor = presentationWidget->textCursor();
        cursor.clearSelection();
        for (int i = 0; i < total; i++) {
            cursor.insertText(value);
        }
        updatePresentationWidget();
    }
}
