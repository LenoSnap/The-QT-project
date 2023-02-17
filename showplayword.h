#ifndef SHOWPLAYWORD_H
#define SHOWPLAYWORD_H

#include <QWidget>
#include <QMouseEvent>
#include <windows.h>        //注意头文件
#include <windowsx.h>
//#include "mainwindow.h"

namespace Ui {
class ShowPlayWord;
}

class ShowPlayWord : public QWidget
{
    Q_OBJECT

public:
    explicit ShowPlayWord(QWidget *parent = 0);
    ~ShowPlayWord();

private slots:

    void ReadWord(QVariant variant);
protected:
    void mousePressEvent(QMouseEvent *e);
    void mouseMoveEvent(QMouseEvent *e);

private:
    Ui::ShowPlayWord *ui;
    int boundaryWidth;
        QPoint clickPos;
};

#endif // SHOWPLAYWORD_H
