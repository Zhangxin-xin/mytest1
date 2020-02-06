#include "widget.h"
#include "ui_widget.h"
#include <QSqlDatabase>
#include <qDebug>
#include <QSqlQuery>
#include <QCheckBox>
#include <QDateTime>

Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);
    //addDatabase中加入代开数据库
    QSqlDatabase db = QSqlDatabase::addDatabase("QMYSQL");
    db.setDatabaseName("sigdata");
    db.setUserName("root");
    db.setPassword("123456");

    if(db.open())
    {
        qDebug()<<"打开数据库成功";
    }
    else
    {
        qDebug()<<"打开数据库失败";
    }
    QSqlQuery q;
    if(q.exec("select * from studentInfo"))
    {
        //q.next();
        //没有这个next时，在刚开始时在一个的上边的位置，在最后一条的时候.next也会有错
        ui->tableWidget->setRowCount(q.size());//设置表中的行数，q.size是返回行数
        int i=0;//第0行
        while (q.next())
        {
            //qDebug()<<q.value(0);
            //将xx放在某行某列
            QCheckBox *c=new QCheckBox("未签到");
            QString sno=q.value(0).toString();
            //q.value(0).toString();将q.value(0)转换成字符串类型

            QSqlQuery q2;
            q2.exec(QString("select sig_time from sigInfo "
                            "where sno = '%1' and date(sig_time)=curdate()"
                            "order by sig_time desc").arg(sno));//curdate() 与date()返回当前日期，格式如下：2013-01-17
            q2.next();
            if(q2.size())
            {
               c->setText("已签到");
               c->setDisabled(true);
               c->setChecked(true);
               ui->tableWidget->setItem(i,4,new QTableWidgetItem(q2.value(0).toString()));
            }


            ui->tableWidget->setItem(i,0,new QTableWidgetItem(sno));
            ui->tableWidget->setItem(i,1,new QTableWidgetItem(q.value(1).toString()));
            ui->tableWidget->setItem(i,2,new QTableWidgetItem(q.value(2).toString()));
            ui->tableWidget->setCellWidget(i,3,c);



            connect(c,QCheckBox::clicked,[=](){
                //1.修改控件本身状态
                c->setText("已签到");
                c->setDisabled(true);
                //2.向表格中添加签到时间
                ui->tableWidget->setItem(i,4,new QTableWidgetItem(QDateTime::currentDateTime().toString()));
                //3.向数据库中插入数据
                QSqlQuery q1;
                //%1,'%2'，占位符(arg函数)
                qDebug()<<q1.exec(QString("insert into sigInfo values('%1',now())").arg(sno));
            });

            i++;
        }
    }
}

Widget::~Widget()
{
    delete ui;
}













