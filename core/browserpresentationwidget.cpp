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

