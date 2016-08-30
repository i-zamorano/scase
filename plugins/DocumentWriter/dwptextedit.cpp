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

#include "dwptextedit.h"

#include <QKeyEvent>
#include <QDebug>
#include <QRect>
#include <QScrollBar>

DWPTextEdit::DWPTextEdit(bool pIgnoreKeyPresses, QTextEdit *parent) :
    QTextEdit(parent)
{
    ignoreKeypresses = pIgnoreKeyPresses;
}

void DWPTextEdit::keyPressEvent(QKeyEvent *e) {
    if (ignoreKeypresses) {
        e->ignore();
    } else {
        QTextEdit::keyPressEvent(e);
    }
}

bool DWPTextEdit::hasPredictionContext() {
    return !toPlainText().isEmpty();
}

std::string DWPTextEdit::getPredictionContext() {
#ifdef SCASE1_PLUGIN_DEBUG_LEVEL_VERBOSE
    qDebug() << "DWPTextEdit::getPredictionContext:" << toPlainText();
#endif
    return toPlainText().toStdString();
}

