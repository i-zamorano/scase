#include "browserpresentationwidget.h"

#include <QtCore/qmath.h>

BrowserPresentationWidget::BrowserPresentationWidget(QWidget *parent) :
    QLabel(parent)
{
}

void BrowserPresentationWidget::setup() {
    setPresentationData("", false);
}

void BrowserPresentationWidget::setupStyle(QString color_, QString backgroundColor_, QString backgroundColorSpecial_, int size) {
    color = color_;
    backgroundColor = backgroundColor_;
    backgroundColorSpecial = backgroundColorSpecial_;
    fontSize = qCeil(size * 0.8);
}

void BrowserPresentationWidget::setPresentationData(QString data, bool isSpecial) {
    setText(data);
    setStyleSheet(QString("BrowserPresentationWidget { font-size:%1px; font-family:Helvetica; font-weight:bold; background-color:#%2 }").arg(QString::number(fontSize), ((isSpecial) ? backgroundColorSpecial : backgroundColor)));
}

