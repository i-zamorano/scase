#ifndef INTERACTIONWIDGET_H
#define INTERACTIONWIDGET_H

#include <QWidget>
#include <QTime>

class InteractionWidget : public QWidget
{
    Q_OBJECT
public:
    explicit InteractionWidget(QWidget *parent = 0);

    inline void setActivationDelay(int activationDelay_) { activationDelay = activationDelay_; }
    inline int getActivationDelay() { return activationDelay; }

signals:
    void userHasEntered();
    void userHasLeft();
    void activated();

public slots:

protected:
    void enterEvent(QEvent *);
    void leaveEvent(QEvent *);
    void paintEvent(QPaintEvent *);

private:
    int activationDelay;

    QTime stopwatch;

};

#endif // INTERACTIONWIDGET_H
