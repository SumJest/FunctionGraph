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
    painter.drawLine(200,0,200,400);
    painter.drawLine(0,200,400,200);
    if(d_flag)
    {
        QPen pen;
        QColor color;
        pen.setWidth(2);
        pen.setColor(color.fromRgb(0,0,255));

        painter.setPen(pen);

        if(!ui->checkBox->isChecked())
        {
            bool ex_flag=false;
            float x = -200;
            float y;
            try{
             y=parse(QString::fromStdString(s_replace(formul,"x",std::to_string(x/10))))*10;
            }catch(std::exception ex){x=0;y=0;}
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
                painter.drawLine(x+200,-y+200,i+200,-ay+200);

                x=i;
                y=ay;
            }
        }else
        {
            QString data = ui->textEdit->toPlainText();
            QStringList strList = data.split(QRegExp("[\n]"),QString::SkipEmptyParts);
            for(int i = 0; i < strList.length();i++)
            {
                formul = strList.at(i).toUtf8().constData();
                adapt(formul);
                bool ex_flag=false;
                float x = -200;
                float y;
                try{
                 y=parse(QString::fromStdString(s_replace(formul,"x",std::to_string(x/10))))*10;
                }catch(std::exception ex){x=0;y=0;}
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
                    painter.drawLine(x+200,-y+200,i+200,-ay+200);

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

    QString stroke = ui->lineEdit->text();
    std::string s = stroke.toUtf8().constData();
    adapt(s);
    formul=s;
    d_flag=true;
    update();
}

void MainWindow::on_checkBox_stateChanged(int arg1)
{
    bool checked = ui->checkBox->isChecked();
    ui->lineEdit->setEnabled(!checked);
    ui->textEdit->setEnabled(checked);
}
