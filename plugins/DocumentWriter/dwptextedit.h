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

#ifndef DWPTEXTEDIT_H
#define DWPTEXTEDIT_H

#include <QTextEdit>

class DWPTextEdit : public QTextEdit
{
    Q_OBJECT
public:
    explicit DWPTextEdit(bool pIgnoreKeyPresses, QTextEdit *parent = 0);

    std::string getPredictionContext();

signals:

public slots:

private:
    bool ignoreKeypresses;
    void keyPressEvent(QKeyEvent *e);
};

#endif // DWPTEXTEDIT_H
