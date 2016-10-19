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
#include <QDebug>
#include <QGraphicsDropShadowEffect>

InteractionWidget::InteractionWidget(QWidget *parent) :
    QWidget(parent)
{
    activationDelay = 500;
    timerInterval = 10;

    setAutoFillBackground(true);
    setStyleSheet("InteractionWidget { background-color:#006600; border-bottom:2px solid #009900; } InteractionWidget:hover { background-color:#009900; border-bottom:2px solid #00DD00; }");

    QGraphicsDropShadowEffect* dropShadowEffect = new QGraphicsDropShadowEffect(this);
    dropShadowEffect->setBlurRadius(5);
    dropShadowEffect->setOffset(0, 8);
    setGraphicsEffect(dropShadowEffect);

    isBlocked = false;

    timer = new QTimer(this);
    timer->setInterval(timerInterval);
    timer->setSingleShot(false);

    progressBar = new QProgressBar(this);
    progressBar->setMinimum(0);
    progressBar->setMaximum(activationDelay);
    progressBar->setTextVisible(false);

    connect(timer, SIGNAL(timeout()), this, SLOT(setProgressValue()));
}

void InteractionWidget::setGeometry(int x, int y, int w, int h)
{
    QWidget::setGeometry(x, y, w, h);
    progressBar->setGeometry(0, 0, w, 25);
}

void InteractionWidget::setProgressValue() {
    runningActivationTime += timerInterval;
    #ifdef SCASE1_DEBUG_LEVEL_VERBOSE
        qDebug() << "InteractionWidget::setProgressValue to " << QString::number(runningActivationTime);
#endif
    progressBar->setValue(runningActivationTime);
}

void InteractionWidget::enterEvent(QEvent *) {
    runningActivationTime = 0;
    stopwatch.start();
    timer->start();
    emit userHasEntered();
}

void InteractionWidget::leaveEvent(QEvent *) {
    if (!isBlocked) {
        isBlocked = true;
#ifdef SCASE1_DEBUG_LEVEL_VERBOSE
        qDebug() << "InteractionWidget::leaveEvent has been called [begin]";
#endif
        if (stopwatch.elapsed() >= activationDelay) {
#ifdef SCASE1_DEBUG_LEVEL_VERBOSE
            qDebug() << "InteractionWidget::leaveEvent has been called with an activation";
#endif
            emit activated();
        } else {
#ifdef SCASE1_DEBUG_LEVEL_VERBOSE
            qDebug() << "InteractionWidget::leaveEvent has been called with an pass";
#endif
            emit userHasLeft();
        }
#ifdef SCASE1_DEBUG_LEVEL_VERBOSE
        qDebug() << "InteractionWidget::leaveEvent has been called [end]";
#endif
    }
    timer->stop();
    progressBar->setValue(0);
    runningActivationTime = 0;
    isBlocked = false;
}

void InteractionWidget::paintEvent(QPaintEvent *) {
    QStyleOption opt;
    opt.init(this);
    QPainter p(this);
    style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);
}
