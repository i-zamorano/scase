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

#ifndef BROWSERPRESENTATIONWIDGET_H
#define BROWSERPRESENTATIONWIDGET_H

#include <QLabel>

#include "interfaces/IBrowserPresentationDelegate.h"

class BrowserPresentationWidget : public QLabel, public IBrowserPresentationDelegate
{
    Q_OBJECT
    Q_INTERFACES(IBrowserPresentationDelegate)

public:
    explicit BrowserPresentationWidget(QWidget *parent = 0);

    void setupStyle(QString color_, QString backgroundColor_, QString backgroundColorSpecial_);
    void setPresentationData(QString data, bool isSpecial);

    void setup();

signals:

public slots:

private:
    QString color;
    QString backgroundColor;
    QString backgroundColorSpecial;
    int fontSize;
    int padding;
};

#endif // BROWSERPRESENTATIONWIDGET_H
