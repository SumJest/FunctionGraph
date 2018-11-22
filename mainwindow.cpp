#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "stack.h"
#include "parser.h"
#include <string>
#include <QMessageBox>
#include <QPainter>
#include <exception>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

std::string formul;
bool d_flag = false;

std::string s_replace(std::string str, const std::string& from, const std::string& to) {
    size_t start_pos=0;

    while((start_pos = str.find(from, start_pos)) != std::string::npos) {
             str.replace(start_pos, from.length(), to);
             start_pos += to.length();
    }
    return str;
}
void adapt(std::string& s)
{
    s="("+s+")";
    char prevToken = s[0];
    char token;
    for(int i = 1; i < s.length();i++)
    {
        token=s[i];
        if(isalpha(token)&&isdigit(prevToken))
        {
            s.insert(i,"*");
        }
        prevToken=token;
    }

    replace(s,"x","(x)");
}
void MainWindow::paintEvent(QPaintEvent *e)
{
    QPainter painter(this);
    QColor color;
    if(ui->checkBox_3->isChecked())
    {

        painter.fillRect(0,2,400,420,color.fromRgb(255,255,255));
        for(int i = 0; i <= 400; i +=10)
        {
            painter.drawLine(i,20,i,420);
            painter.drawLine(0,i+20,400,i+20);
        }
        QPen pen;
        pen.setWidth(2);
        painter.setPen(pen);
    }
    painter.drawLine(200,20,200,420);
    painter.drawLine(0,220,400,220);
    painter.drawLine(400,20,400,420);
    painter.drawLine(0,420,400,420);
    painter.drawLine(1,21,400,21);
    painter.drawLine(1,21,1,420);
    if(d_flag)
    {
        QPen pens[4];
        int width = ui->label_3->text().toInt();
        pens[0].setWidth(width);
        pens[1].setWidth(width);
        pens[2].setWidth(width);
        pens[3].setWidth(width);
        pens[0].setColor(color.fromRgb(50,60,255));
        pens[1].setColor(color.fromRgb(255,70,70));
        pens[2].setColor(color.fromRgb(255,255,0));
        pens[3].setColor(color.fromRgb(0, 255, 0));

        painter.setPen(pens[rand()%4]);
        if(!ui->checkBox_2->isChecked())painter.setPen(pens[0]);
        if(!ui->checkBox->isChecked())
        {
            QString stroke = ui->lineEdit->text();
            std::string s = stroke.toUtf8().constData();
            adapt(s);
            formul=s;
            bool ex_flag=false;
            float x = -200;
            float y;
            bool it = true;
            while(it)
            {
                it = false;
                try{
                 y=parse(QString::fromStdString(s_replace(formul,"x",std::to_string(x/10))))*10;
                }catch(std::exception ex){x++;y=0;it=true;}
            }
            for(float i = x;i<=200;i+=1)
            {
                float ay;
                try {
                   ay = parse(QString::fromStdString(s_replace(formul,"x",std::to_string(i/10))))*10;
                } catch (std::exception ex) {
                    x=i;
                    ex_flag=true;
                    continue;
                }
                if(ex_flag) y=ay; ex_flag=false;
                painter.drawLine(x+200,-y+220,i+200,-ay+220);

                x=i;
                y=ay;
            }
        }else
        {
            QString data = ui->textEdit->toPlainText();
            QStringList strList = data.split(QRegExp("[\n]"),QString::SkipEmptyParts);
            for(int i = 0; i < strList.length();i++)
            {
                painter.setPen(pens[i%4]);
                if(!ui->checkBox_2->isChecked())painter.setPen(pens[0]);
                formul = strList.at(i).toUtf8().constData();
                adapt(formul);
                bool ex_flag=false;
                float x = -200;
                float y;
                bool it = true;
                while(it)
                {
                    it = false;
                    try{
                     y=parse(QString::fromStdString(s_replace(formul,"x",std::to_string(x/10))))*10;
                    }catch(std::exception ex){x++;y=0;it=true;}
                }
                for(float i = x;i<=200;i+=1)
                {
                    float ay;
                    try {
                       ay = parse(QString::fromStdString(s_replace(formul,"x",std::to_string(i/10))))*10;
                    } catch (std::exception ex) {
                        x=i;
                        ex_flag=true;
                        continue;
                    }
                    if(ex_flag) y=ay; ex_flag=false;
                    painter.drawLine(x+200,-y+220,i+200,-ay+220);

                    x=i;
                    y=ay;
                }
            }
        }
        d_flag=false;
    }
}
void MainWindow::on_pushButton_clicked()
{
    d_flag=true;
    update();
}

void MainWindow::on_checkBox_stateChanged(int arg1)
{
    bool checked = ui->checkBox->isChecked();
    ui->lineEdit->setEnabled(!checked);
    ui->textEdit->setEnabled(checked);
}

void MainWindow::on_horizontalSlider_valueChanged(int value)
{
    ui->label_3->setText(QString::number(value));
}

void MainWindow::on_checkBox_3_stateChanged(int arg1)
{
    update();
}

void MainWindow::on_action_triggered()
{
    QMessageBox mbox;
    mbox.setText("Автор: Роман Фахрутдинов\nE-mail: roman@sumjest.ru");
    mbox.exec();
}
