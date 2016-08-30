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

#include "browserpresentationwidget.h"

#include <QtCore/qmath.h>
#include <QDebug>
#include <QGraphicsDropShadowEffect>

BrowserPresentationWidget::BrowserPresentationWidget(QWidget *parent) :
    QLabel(parent)
{
}

void BrowserPresentationWidget::setup() {
    QGraphicsDropShadowEffect* dropShadowEffect = new QGraphicsDropShadowEffect(this);
    dropShadowEffect->setBlurRadius(5);
    dropShadowEffect->setOffset(0, 8);
    setGraphicsEffect(dropShadowEffect);
    setPresentationData("", false);
}

void BrowserPresentationWidget::setupStyle(QString color_, QString backgroundColor_, QString backgroundColorSpecial_) {
    color = color_;
    backgroundColor = backgroundColor_;
    backgroundColorSpecial = backgroundColorSpecial_;

    int forcedSize = this->size().height();
    fontSize = qCeil(forcedSize * 0.8);
    padding = qCeil(forcedSize * 0.1);

    if (fontSize > 80) {
        fontSize = 80;
    }

#ifdef SCASE1_DEBUG_LEVEL_VERBOSE
    qDebug() << "BrowserPresentationWidget::setupStyle:forcedSize" << QString::number(forcedSize);
    qDebug() << "BrowserPresentationWidget::setupStyle:fontSize" << QString::number(fontSize);
    qDebug() << "BrowserPresentationWidget::setupStyle:padding" << QString::number(padding);
#endif
}

void BrowserPresentationWidget::setPresentationData(QString data, bool isSpecial) {
    setText(data);
    setStyleSheet(QString("BrowserPresentationWidget { color:#%4; font-size:%1px; font-family:Helvetica; font-weight:bold; background-color:#%2; padding:%3px; border-top:2px solid #000000; border-bottom:2px solid #000000; }").arg(QString::number(fontSize), ((isSpecial) ? backgroundColorSpecial : backgroundColor), QString::number(padding), color));
}

