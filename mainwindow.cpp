#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_browseButton_clicked()
{
    QString fileName = QFileDialog::getOpenFileName(this,
            tr("Executable"), "",
            tr("All Files (*)"));
    if (fileName.isEmpty())
        return;
    else {
        ui->executableText->setText(fileName);
        return;
    }
}

void MainWindow::on_runButton_clicked()
{
    QString program = "faketime";
    QStringList parameters;

    parameters << ui->dateTimeEdit->dateTime().toString("yyyy-MM-dd HH:mm:ss");

    if (ui->wineCheckBox->isChecked())
    {
        parameters << "wine";
    }
    parameters << ui->executableText->text();
    parameters << ui->parametersText->text().split(" ");
    QProcess process;
    process.startDetached(program, parameters);
    process.waitForStarted(-1);
}

void MainWindow::on_seedText_textEdited(const QString &arg1)
{
    bool check;
    QDate date(2000, 1, 1);
    QDate end(2000, 12, 31);
    for (; date <= end; date = date.addDays(1))
    {
        u_int16_t days = date.dayOfYear();
        for (u_int8_t hour = 0; hour < 24; hour++)
                {
            for (u_int8_t minute = 0; minute < 60; minute++)
            {
                u_int32_t v = 1440 * days + 960 * (hour / 10) + 60 * (hour % 10) + 16 * (minute / 10) + (minute % 10);
                v = (v >> 16) ^ (v & 0xffff);
                if (v == arg1.toUInt(&check,16))
                {
                    ui->dateTimeEdit->setDate(date);
                    ui->dateTimeEdit->setTime(QTime(hour,minute,0));
                    return;
                }
            }
        }
    }
}

void MainWindow::on_dateTimeEdit_dateTimeChanged(const QDateTime &dateTime)
{
    int d = dateTime.date().dayOfYear() - 1;
    int h = dateTime.time().hour();
    int m = dateTime.time().minute();
    u_int32_t v = 1440 * d + 960 * (h / 10) + 60 * (h % 10) + 16 * (m / 10) + (m % 10) + 0x5A0;
    v = (v >> 16) ^ (v & 0xFFFF);
    ui->seedText->setText(QString::number(v,16));
}
