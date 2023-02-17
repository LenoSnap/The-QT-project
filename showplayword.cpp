#include "showplayword.h"
#include "ui_showplayword.h"
#include"mainwindow.h"

ShowPlayWord::ShowPlayWord(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ShowPlayWord)
{
    ui->setupUi(this);
    setWindowFlags(Qt::FramelessWindowHint);
   // setAttribute(Qt::WA_TranslucentBackground, true);
    //this->setAttribute(Qt::WA_WState_WindowOpacitySet);
    setWindowOpacity(0.5);
}

ShowPlayWord::~ShowPlayWord()
{
    delete ui;
}
void ShowPlayWord::mousePressEvent(QMouseEvent *e)
{
    if(e->button()==Qt::LeftButton)
        clickPos=e->pos();
}
void ShowPlayWord::mouseMoveEvent(QMouseEvent *e)
{
    if(e->buttons()&Qt::LeftButton  //左键点击并且移动
            && e->pos().x()>=0      //范围在窗口的上面部分
            && e->pos().y()>=0
            && e->pos().x()<= geometry().width()
            && e->pos().y() <= geometry().height())
    {
        move(e->pos()+pos()-clickPos);  //移动窗口
    }
}
void ShowPlayWord::ReadWord(QVariant variant)
{

    WordObjectInfo info=variant.value<WordObjectInfo>();
  //  qDebug()<<info.previousWord.split("]").at(1);
   // qDebug()<<info.currentWord.split("]").at(1);
   // qDebug()<<info.nextWord.split("]").at(1);

    ui->label_previous->setText(info.previousWord.split("]").at(1));
    ui->label_current->setText(info.currentWord.split("]").at(1));
    ui->label_next->setText(info.nextWord.split("]").at(1));

    //ui->label_current->setText("测试文字");
}
