#include "browseritem.h"

BrowserItem::BrowserItem(QObject *parent) :
    QObject(parent)
{
    presentationData = "";
    levelBelow_ = NULL;
    moduleName = NULL;
    actionName = NULL;
    pluginDelegate = NULL;
    container = NULL;
    isSpecial_ = false;
}

QString BrowserItem::getPresentationData() {
    return presentationData;
}

void BrowserItem::setPresentationData(const QString presentationData_) {
    presentationData = presentationData_;
}

BrowserLevel *BrowserItem::getLevelBelow() {
    return static_cast<BrowserLevel *>(levelBelow_);
}

void BrowserItem::setLevelBelow(BrowserLevel *levelBelow) {
    levelBelow->setContainer(this);
    IBrowserItem::setLevelBelow(levelBelow);
}

bool BrowserItem::hasAction() {
    return ((moduleName != NULL) && (actionName != NULL));
}

QString BrowserItem::getActionModule() {
    return QString(*moduleName);
}

QString BrowserItem::getActionName() {
    return QString(*actionName);
}

void BrowserItem::setAction(const QString moduleName_, const QString actionName_) {
    moduleName = new QString(moduleName_);
    actionName = new QString(actionName_);
}

