#ifndef IBROWSERPRESENTATIONDELEGATE_H
#define IBROWSERPRESENTATIONDELEGATE_H

#include <QString>
#include <QtPlugin>

class IBrowserPresentationDelegate {

public:
    virtual void setPresentationData(QString data, bool isSpecial) = 0;

};

#define IBROWSER_PRESENTATION_DELEGATE_SID "net.adapar.SCASE1.IBrowserPresentationDelegate/1.0"

Q_DECLARE_INTERFACE(IBrowserPresentationDelegate, IBROWSER_PRESENTATION_DELEGATE_SID)

#endif // IBROWSERPRESENTATIONDELEGATE_H
