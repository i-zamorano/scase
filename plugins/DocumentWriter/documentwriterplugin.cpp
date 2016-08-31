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

#include "documentwriterplugin.h"

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
#include <QtCore/qmath.h>

#include <locale>

DocumentWriterPlugin::DocumentWriterPlugin()
#ifdef SCASE1_PLUGIN_DOCUMENTWRITER_PREDICTION_ENABLED
    : presageCallback(presageStdContext),
      presage(&presageCallback, "plugins/presage/presage.xml")
#endif
{
    QTextCodec::setCodecForLocale(QTextCodec::codecForName("utf-8"));

    browserItemDelegate = NULL;
    rootLevel = NULL;

    autosave = true;

#ifdef SCASE1_PLUGIN_DEBUG_LEVEL_VERBOSE
    qDebug() << "DocumentWriterPlugin::DocumentWriterPlugin:pluginDirPath?" << getPluginPath();
#endif

    settings = new QSettings(getPluginPath() + SCASE1_PLUGIN_DOCUMENTWRITER_SETTINGS_FILE + ".ini", QSettings::IniFormat, this);

    presentationWidget = new DWPTextEdit(settings->value("presentation/ignore_keypresses", false).toBool());

    //presentationWidget->setAlignment(Qt::AlignTop | Qt::AlignLeft);
    presentationWidget->setWordWrapMode(QTextOption::WrapAtWordBoundaryOrAnywhere);
    presentationWidget->setUndoRedoEnabled(true);
    presentationWidget->ensureCursorVisible();
    presentationWidget->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

#ifdef SCASE1_PLUGIN_DOCUMENTWRITER_PREDICTION_ENABLED
    predictedItemsAdded = 0;
#endif

    connect(presentationWidget, SIGNAL(textChanged()), this, SLOT(textHasChanged()));
}

void DocumentWriterPlugin::textHasChanged() {
    saveRecentCache();
}

void DocumentWriterPlugin::show(QStackedWidget *container) {
    container->setCurrentWidget(getOutputWidget());
    presentationWidget->show();
    presentationWidget->raise();
    presentationWidget->setFocus();
    setupOutputWidget();
}

void DocumentWriterPlugin::hide() {
    if (autosave) {
        saveCurrentVersion();
    }
    saveRecentCache();
    presentationWidget->hide();
}

void DocumentWriterPlugin::saveCurrentVersion() {
    QDate currentDate = QDate::currentDate();

    QString path = QDir::toNativeSeparators(QString("%1%2/%3").arg(documentPath, currentDate.toString("yyyy"), currentDate.toString("MM")));
    QString filename = QString("version-%1-%2.txt").arg(currentDate.toString("dd"), QTime::currentTime().toString("HHmmss"));
    QString filepath = QDir::toNativeSeparators(QString("%1/%2").arg(path, filename));

#ifdef SCASE1_PLUGIN_DEBUG_LEVEL_VERBOSE
    qDebug() << "DocumentWriterPlugin::saveCurrentVersion:currentPath?" << QDir::currentPath();
    qDebug() << "DocumentWriterPlugin::saveCurrentVersion:path?" << path;
    qDebug() << "DocumentWriterPlugin::saveCurrentVersion:filename?" << filename;
#endif

    QDir dir(QDir::currentPath());
    dir.mkpath(path);

    saveContentsTo(filepath);
}

void DocumentWriterPlugin::saveRecentCache() {
#ifdef SCASE1_PLUGIN_DEBUG_LEVEL_VERBOSE
        qDebug() << "DocumentWriterPlugin::saveRecentCache";
#endif
    saveContentsTo(getRecentCacheFilename());
}

QString DocumentWriterPlugin::getRecentCacheFilename() {
    return QString("%1recent.txt").arg(documentPath);
}

void DocumentWriterPlugin::setDocumentPath(QString configuredPath) {
    documentPath = QDir::toNativeSeparators(QString("%1/%2/").arg(userPath, configuredPath));
#ifdef SCASE1_PLUGIN_DEBUG_LEVEL_VERBOSE
    qDebug() << "DocumentWriterPlugin::setDocumentPath:documentPath?" << documentPath;
#endif
}

QString DocumentWriterPlugin::getRecentCache() {
    return getContentsFrom(getRecentCacheFilename());
}

QString DocumentWriterPlugin::getContentsFrom(QString filepath) {
    QFile inFile(filepath);
    QString content = "";

    if (inFile.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QTextStream in(&inFile);
        content = in.readAll();
#ifdef SCASE1_PLUGIN_DEBUG_LEVEL_VERBOSE
    } else {
        qDebug() << "DocumentWriterPlugin::getContentsFrom:could not open file for reading";
    }
#else
    }
#endif

    return content;
}

void DocumentWriterPlugin::saveContentsTo(QString filepath) {
#ifdef SCASE1_PLUGIN_DEBUG_LEVEL_VERBOSE
    qDebug() << "DocumentWriterPlugin::saveContentsTo:filename = " << filepath;
#endif
    QFile outputFile(filepath);

    if (outputFile.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QTextStream out(&outputFile);
        out << presentationWidget->toPlainText();
        outputFile.flush();
        outputFile.close();
#ifdef SCASE1_PLUGIN_DEBUG_LEVEL_VERBOSE
    } else {
        qDebug() << "DocumentWriterPlugin::saveContentsTo:could not open file for saving";
    }
#else
    }
#endif
}

void DocumentWriterPlugin::setupOutputWidget() {
    QString configuredColor = settings->value("presentation/color", "000000").toString().trimmed();
    QString configuredBackgroundColor = settings->value("presentation/background_color", "ffffff").toString().trimmed();
    QString configuredSize = settings->value("presentation/size", "100%").toString().trimmed();
    int configuredLines = settings->value("presentation/lines", 5).toInt();

    QString configuredPath = settings->value("storage/document_path", "documents").toString().trimmed();
    bool configuredAutoSave = settings->value("storage/autosave", true).toBool();
    bool configuredAutoLoad = settings->value("storage/autoload", true).toBool();

#ifdef SCASE1_PLUGIN_DEBUG_LEVEL_VERBOSE
    QStringList keys = settings->allKeys();
    foreach (QString key, keys) {
        qDebug() << "DocumentWriterPlugin::setupOutputWidget:key?" << key.toUtf8();
    }
    qDebug() << "DocumentWriterPlugin::setupOutputWidget:configuredColor?" << configuredColor;
    qDebug() << "DocumentWriterPlugin::setupOutputWidget:configuredBackgroundColor?" << configuredBackgroundColor;
    qDebug() << "DocumentWriterPlugin::setupOutputWidget:configuredSize?" << configuredSize;
    qDebug() << "DocumentWriterPlugin::setupOutputWidget:configuredLines?" << configuredLines;
    qDebug() << "DocumentWriterPlugin::setupOutputWidget:configuredPath?" << configuredPath;
    qDebug() << "DocumentWriterPlugin::setupOutputWidget:configuredAutoSave?" << configuredAutoSave;
    qDebug() << "DocumentWriterPlugin::setupOutputWidget:configuredAutoLoad?" << configuredAutoLoad;
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
    qDebug() << "DocumentWriterPlugin::setupOutputWidget:sizeIsProportional?" << (sizeIsProportional ? "yes" : "no");
    qDebug() << "DocumentWriterPlugin::setupOutputWidget:containerSize?" << containerSize;
    qDebug() << "DocumentWriterPlugin::setupOutputWidget:size?" << size;
#endif

    if (sizeIsProportional) {
        size = qCeil((containerSize * size) / 100.0f);
#ifdef SCASE1_PLUGIN_DEBUG_LEVEL_VERBOSE
        qDebug() << "DocumentWriterPlugin::setupOutputWidget:size(calculated)?" << size;
#endif
    }

    if (size > containerSize) {
        size = containerSize - 300;
    }

    int fontSize = qCeil(size / (configuredLines * 1.5f));

    QString presentationWidgetStyle = QString("DWPTextEdit { padding:10px; font-family: Helvetica, Arial; font-size: %1px; background-color: #%2; color: #%3; }").arg(QString::number(fontSize), configuredBackgroundColor, configuredColor);

#ifdef SCASE1_PLUGIN_DEBUG_LEVEL_VERBOSE
    qDebug() << "DocumentWriterPlugin::setupOutputWidget:fontSize?" << fontSize;
    qDebug() << "DocumentWriterPlugin::setupOutputWidget:presentationWidgetStyle?" << presentationWidgetStyle;
#endif

    presentationWidget->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    presentationWidget->setStyleSheet(presentationWidgetStyle);
    presentationWidget->setCursorWidth(10);
    presentationWidget->setFixedHeight(size);

    autosave = configuredAutoSave;

    setDocumentPath(configuredPath);

    if (configuredAutoLoad) {
        presentationWidget->setText(getRecentCache());
    }

    QTextCursor cursor = presentationWidget->textCursor();
    QTextBlockFormat blockFormat = cursor.blockFormat();
    blockFormat.setLineHeight(150, QTextBlockFormat::ProportionalHeight);
    cursor.setBlockFormat(blockFormat);
    cursor.movePosition(QTextCursor::End);
    cursor.clearSelection();
    presentationWidget->setTextCursor(cursor);
}

QString DocumentWriterPlugin::getPluginPath() {
    return QString("plugins/");
}

void DocumentWriterPlugin::invokeActionPrivate(const QString actionName_) {
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

QString DocumentWriterPlugin::getBrowserTree() {
    QString browserTreeFile = getPluginPath() + "lang/" + settings->value("presentation/language", "en").toString().trimmed() + "/" + SCASE1_PLUGIN_DOCUMENTWRITER_BROWSER_TREE_FILE + ".xml";
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

QWidget *DocumentWriterPlugin::getOutputWidget() {
    return presentationWidget;
}

QString DocumentWriterPlugin::getName() {
    return QString(SCASE1_PLUGIN_DOCUMENTWRITER_NAME);
}

void DocumentWriterPlugin::write_special(QString value, QString repetitions) {
    QString dataToAdd = "";
    if (value == "space") {
        dataToAdd = " ";
    } else if (value == "period") {
        dataToAdd = ".";
    } else if (value == "comma") {
        dataToAdd = ",";
    } else if (value == "semicolon") {
        dataToAdd = ";";
    } else if (value == "colon") {
        dataToAdd = ":";
    } else if (value == "enter") {
        dataToAdd = "\n";
    } else if (value == "tab") {
        dataToAdd = "\t";
    }
    if (dataToAdd.size() > 0) {
        write(dataToAdd, repetitions);
    }
}

void DocumentWriterPlugin::write_prediction(QString value) {
    write(value, "1", true);
}

void DocumentWriterPlugin::write(QString value, QString repetitions, bool isPrediction) {
#ifdef SCASE1_PLUGIN_DEBUG_LEVEL_VERBOSE
    qDebug() << "value:" << value;
    qDebug() << "repetitions:" << repetitions;
#endif

    if (isPrediction) {
        QString selectedText;
        QTextCursor cursor = presentationWidget->textCursor();

        bool foundPrefix = false;
        int checkedLength = 0;
        int predictionLength = value.length();

        cursor.movePosition(QTextCursor::End);
        cursor.clearSelection();

        while (!foundPrefix && checkedLength <= predictionLength && cursor.position() > 0) {
            cursor.movePosition(QTextCursor::PreviousCharacter, QTextCursor::KeepAnchor);
            selectedText = cursor.selectedText();
#ifdef SCASE1_PLUGIN_DEBUG_LEVEL_VERBOSE
            qDebug() << "write_prediction:cursor.position= " << QString::number(cursor.position());
            qDebug() << "write_prediction:selectedText = " << selectedText;
            qDebug() << "write_prediction:value = " << value;
            checkedLength = selectedText.length();
#endif
            if (value.indexOf(selectedText, 0, Qt::CaseInsensitive) == 0) {
#ifdef SCASE1_PLUGIN_DEBUG_LEVEL_VERBOSE
                qDebug() << "write_prediction:foundPrefix!";
#endif
                foundPrefix = true;
                cursor.removeSelectedText();
                cursor.insertText(value);
                cursor.movePosition(QTextCursor::End);
                cursor.clearSelection();
            }
        }

        if (!foundPrefix) {
            cursor.movePosition(QTextCursor::End);
            cursor.clearSelection();
            cursor.insertText(value);
        }

        cursor.movePosition(QTextCursor::End);
        cursor.clearSelection();

        presentationWidget->setTextCursor(cursor);

        updatePresentationWidget();
    } else {
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
}

void DocumentWriterPlugin::delete_previous_character() {
    delete_content(QTextCursor::PreviousCharacter);
}

void DocumentWriterPlugin::delete_previous_word() {
    delete_content(QTextCursor::PreviousWord);
}

void DocumentWriterPlugin::delete_all() {
    presentationWidget->moveCursor(QTextCursor::End);
    delete_content(QTextCursor::Start);
}

void DocumentWriterPlugin::delete_content(QTextCursor::MoveOperation moveOperation) {
    QTextCursor cursor = presentationWidget->textCursor();
    cursor.clearSelection();
    cursor.movePosition(moveOperation, QTextCursor::KeepAnchor);
    cursor.removeSelectedText();
    cursor.clearSelection();
    presentationWidget->setTextCursor(cursor);
    updatePresentationWidget();
}

void DocumentWriterPlugin::undo() {
    presentationWidget->undo();
    updatePresentationWidget();
}

void DocumentWriterPlugin::move_cursor(QString direction) {
    move_cursor(direction, "");
}

void DocumentWriterPlugin::move_cursor(QString direction, QString type) {
    QTextCursor::MoveOperation moveOperation = QTextCursor::NoMove;

    if (direction == "up") {
        moveOperation = QTextCursor::Up;
    } else if (direction == "down") {
        moveOperation = QTextCursor::Down;
    } else if (direction == "left") {
        if (type == "character") {
            moveOperation = QTextCursor::Left;
        } else if (type == "word") {
            moveOperation = QTextCursor::WordLeft;
        }
    } else if (direction == "right") {
        if (type == "character") {
            moveOperation = QTextCursor::Right;
        } else if (type == "word") {
            moveOperation = QTextCursor::WordRight;
        }
    }
    QTextCursor cursor = presentationWidget->textCursor();
    cursor.clearSelection();
    cursor.movePosition(moveOperation, QTextCursor::MoveAnchor);
    presentationWidget->setTextCursor(cursor);
    updatePresentationWidget();
}

void DocumentWriterPlugin::setBrowserItemDelegatePrivate(IBrowserItem *delegate) {
    Q_UNUSED(delegate);
    if ((browserItemDelegate != NULL) && browserItemDelegate->hasLevelBelow()) {
        rootLevel = browserItemDelegate->getLevelBelow();
    } else {
        rootLevel = NULL;
    }
}

bool DocumentWriterPlugin::isContextValidForPrediction(std::string context) {
    return isalnum((int)context.at(context.length() - 1));
}

void DocumentWriterPlugin::updatePresentationWidget() {
#ifdef SCASE1_PLUGIN_DOCUMENTWRITER_PREDICTION_ENABLED
    if (predictedItemsAdded > 0) {
        if (rootLevel != NULL) {
            for (int i = 0; i < predictedItemsAdded; i++) {
                rootLevel->removeItemAtPos(0);
            }
        }
        predictedItemsAdded = 0;
    }
#endif

#ifdef SCASE1_PLUGIN_DOCUMENTWRITER_PREDICTION_ENABLED
    std::vector< std::string > predictions;

    if (presentationWidget->hasPredictionContext()) {
#ifdef SCASE1_PLUGIN_DEBUG_LEVEL_VERBOSE
    qDebug() << "DocumentWriterPlugin::updatePresentationWidget has prediction context";
#endif
        std::string context = presentationWidget->getPredictionContext();
#ifdef SCASE1_PLUGIN_DEBUG_LEVEL_VERBOSE
    qDebug() << "DocumentWriterPlugin::updatePresentationWidget read context from presentationWidget";
#endif

        if (isContextValidForPrediction(context)) {
#ifdef SCASE1_PLUGIN_DEBUG_LEVEL_VERBOSE
    qDebug() << "DocumentWriterPlugin::updatePresentationWidget prediction context is valid";
#endif
            presageStdContext = context;
            predictions = presage.predict();
        } else {
#ifdef SCASE1_PLUGIN_DEBUG_LEVEL_VERBOSE
    qDebug() << "DocumentWriterPlugin::updatePresentationWidget prediction context is NOT valid";
#endif
            predictions.clear();
        }
#ifdef SCASE1_PLUGIN_DEBUG_LEVEL_VERBOSE
    } else {
    qDebug() << "DocumentWriterPlugin::updatePresentationWidget does NOT have prediction context";
#endif
    }

    if (predictions.size() > 0) {
        if (rootLevel != NULL) {
            int maxPredictions = ( predictions.size() < 3 ) ? predictions.size() : 3;

            predictedItemsAdded = maxPredictions;

            QString prediction;

            for (int i = 0; i < maxPredictions; i++) {
                prediction = QString::fromUtf8(predictions[i].c_str()).toUpper();
#ifdef SCASE1_PLUGIN_DEBUG_LEVEL_VERBOSE
                qDebug() << "DocumentWriterPlugin.prediction:" << prediction;
#endif
                browserDelegate->addItemToLevel(rootLevel, 0, prediction, getName(), QString("write_prediction,%1").arg(prediction), true);
            }
        }
    }
#endif
}
