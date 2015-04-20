#include "browserlevel.h"

BrowserLevel::BrowserLevel(QObject *parent) :
    QObject(parent)
{
    reset();
    isStop_ = false;
    container = NULL;
}

void BrowserLevel::insertItem(IBrowserItem *item) {
    insertItem(item, items.size());
}

void BrowserLevel::insertItem(IBrowserItem *item, int pos) {
    if (item != NULL) {
        if (pos < 0) {
            pos = 0;
        } else if (pos > items.size()) {
            pos = items.size();
        }
        items.insert(pos, item);
        item->setContainer(this);
    }
}

void BrowserLevel::removeItemAtPos(int pos) {
    if (!items.isEmpty()) {
        items.removeAt(pos);
    }
}

void BrowserLevel::reset() {
    currentPosition = 0;
}

void BrowserLevel::next() {
    currentPosition++;
}

bool BrowserLevel::atEnd() {
    return (currentPosition >= items.size());
}

BrowserItem *BrowserLevel::getCurrentItem() {
    if (atEnd() || (currentPosition < 0)) {
        return NULL;
    }
    return static_cast<BrowserItem *>(items.at(currentPosition));
}
