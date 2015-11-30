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

#include "interactionwidget.h"

#include <QStyleOption>
#include <QPainter>

InteractionWidget::InteractionWidget(QWidget *parent) :
    QWidget(parent)
{
    activationDelay = 500;
    setAutoFillBackground(true);
    setStyleSheet("InteractionWidget { background-color:#999999 } InteractionWidget:hover { background-color:#009900 }");
}

void InteractionWidget::enterEvent(QEvent *) {
    stopwatch.start();
    emit userHasEntered();
}

void InteractionWidget::leaveEvent(QEvent *) {
    if (stopwatch.elapsed() >= activationDelay) {
        emit activated();
    } else {
        emit userHasLeft();
    }
}

void InteractionWidget::paintEvent(QPaintEvent *) {
    QStyleOption opt;
    opt.init(this);
    QPainter p(this);
    style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);
}
