#include "dwptextedit.h"

#include <QKeyEvent>

DWPTextEdit::DWPTextEdit(QTextEdit *parent) :
    QTextEdit(parent)
{
}

void DWPTextEdit::keyPressEvent(QKeyEvent *e) {
    e->ignore();
}
