#ifndef BROWSERLEVEL_H
#define BROWSERLEVEL_H

#include <QObject>

#include <QList>

#include "interfaces/IBrowserItem.h"
#include "interfaces/IBrowserLevel.h"

class BrowserLevel;

#include "browseritem.h"

class BrowserLevel : public QObject, public IBrowserLevel
{
    Q_OBJECT
    Q_INTERFACES(IBrowserLevel)

public:
    explicit BrowserLevel(QObject *parent = 0);

    void insertItem(IBrowserItem *item, int pos);
    void insertItem(IBrowserItem *item);
    void removeItemAtPos(int pos);

    void reset();
    void next();
    bool atEnd();

    inline bool isStop() { return isStop_; }
    inline void setIsStop(bool value) { isStop_ = value; }

    BrowserItem *getCurrentItem();

signals:

public slots:

private:
    QList<IBrowserItem *>items;

    int currentPosition;

    bool isStop_;
};

#endif // BROWSERLEVEL_H
