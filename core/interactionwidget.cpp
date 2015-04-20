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
