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

#ifndef INTERACTIONWIDGET_H
#define INTERACTIONWIDGET_H

#include <QWidget>
#include <QTime>
#include <QTimer>
#include <QProgressBar>

class InteractionWidget : public QWidget
{
    Q_OBJECT
public:
    explicit InteractionWidget(QWidget *parent = 0);

    inline void setActivationDelay(int activationDelay_) { activationDelay = activationDelay_; }
    inline int getActivationDelay() { return activationDelay; }

    inline void setRefractoryPeriod(int refractoryPeriod_) { refractoryPeriod = refractoryPeriod_; }

    void setGeometry(int x, int y, int w, int h);

signals:
    void userHasEntered();
    void userHasLeft();
    void activated();

public slots:
    void setProgressValue();
    void block();
    void unblock();

protected:
    void enterEvent(QEvent *);
    void leaveEvent(QEvent *);
    void paintEvent(QPaintEvent *);

private:
    int activationDelay;
    int timerInterval;
    int runningActivationTime;
    int refractoryPeriod;

    bool isBlocked;
    bool hasEntered;

    QTime stopwatch;
    QTimer *timer;
    QProgressBar *progressBar;

};

#endif // INTERACTIONWIDGET_H
