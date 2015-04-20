#ifndef DWPTEXTEDIT_H
#define DWPTEXTEDIT_H

#include <QTextEdit>

class DWPTextEdit : public QTextEdit
{
    Q_OBJECT
public:
    explicit DWPTextEdit(QTextEdit *parent = 0);

signals:

public slots:

private:
    void keyPressEvent(QKeyEvent *e);
};

#endif // DWPTEXTEDIT_H
