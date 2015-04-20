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

    void setupStyle(QString color_, QString backgroundColor_, QString backgroundColorSpecial_, int size);
    void setPresentationData(QString data, bool isSpecial);

    void setup();

signals:

public slots:

private:
    QString color;
    QString backgroundColor;
    QString backgroundColorSpecial;
    int fontSize;
};

#endif // BROWSERPRESENTATIONWIDGET_H
