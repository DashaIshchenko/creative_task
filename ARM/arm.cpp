#include "arm.h"
#include "ui_arm.h"

#include <QDebug>
#include <QSqlError>
#include <QSqlRecord>
#include <QTableView>
#include <QDate>
#include <QFile>
#include <QTextStream>

#include <string>
#include <algorithm>


ARM::ARM(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::ARM)
{
    ui->setupUi(this);

    ui->btnBirthday->setStyleSheet("background-color:" + QColor(91, 161, 74).name());
    ui->tabWidget->setStyleSheet("background-color:" + QColor(25, 61, 23).name());
    ui->btnMeeting->setStyleSheet("background-color:" + QColor(91, 161, 74).name());
    ui->btnSendEmail->setStyleSheet("background-color:" + QColor(91, 161, 74).name());
    ui->btnSkipped->setStyleSheet("background-color:" + QColor(91, 161, 74).name());
    ui->boxMarks->setStyleSheet("background-color:" + QColor(91, 161, 74).name());
    ui->boxStudent->setStyleSheet("background-color:" + QColor(91, 161, 74).name());
    ui->btnUpdateClass->setStyleSheet("background-color:" + QColor(91, 161, 74).name());
    ui->listMarks->setStyleSheet("background-color:" + QColor(255, 255, 255).name());
    ui->listRating->setStyleSheet("background-color:" + QColor(255, 255, 255).name());
    ui->tableSkipped->setStyleSheet("background-color:" + QColor(255, 255, 255).name());
    ui->inputForEmail->setStyleSheet("background-color:" + QColor(255, 255, 255).name());
    ui->textEmail->setStyleSheet("background-color:" + QColor(255, 255, 255).name());
    ui->viewBirthday->setStyleSheet("background-color:" + QColor(255, 255, 255).name());
    ui->showMeeting->setStyleSheet("background-color:" + QColor(255, 255, 255).name());
    ui->getMeeting->setStyleSheet("background-color:" + QColor(255, 255, 255).name());

    ui->label->setStyleSheet("color: rgb(255, 255, 255)");
    ui->label_2->setStyleSheet("color: rgb(255, 255, 255)");
    ui->label_3->setStyleSheet("color: rgb(255, 255, 255)");
    ui->label_4->setStyleSheet("color: rgb(255, 255, 255)");
    ui->label_5->setStyleSheet("color: rgb(255, 255, 255)");
    ui->label_6->setStyleSheet("color: rgb(255, 255, 255)");


/// DATABASE ///
    db = QSqlDatabase::addDatabase("QSQLITE");
    //db.setHostName("127.0.0.1"); //idk. it's local server
    db.setDatabaseName("C:\\SQLiteStudio\\QT_teacher"); // specify file directory
    db.setUserName("root"); //idk still
    db.setPassword("123456");
    if (!db.open()){
        qDebug() << db.lastError().text();
        return;
    }
    else {
        qDebug() << "Successfully connected to the DB\n";
    }
/// EMAIL ///
    connect(ui->inputForEmail, &QLineEdit::textChanged, this, &ARM::on_UserInputLine_textChanged);


/// MARKS FOR SUBJECTS ///
    ui->boxMarks->addItem("Английский");
    ui->boxMarks->addItem("Русский");
    ui->boxMarks->addItem("Математика");
    ui->boxMarks->addItem("Литература");

/// MARKS FOR STUDENTS ///

    QSqlQuery query = QSqlQuery(db);

    query.exec("SELECT students_info.Фамилия FROM students_info");
    while (query.next()){
        names.push_back(query.value(0).toString());
    }

    for (auto it = names.begin(); it != names.end(); ++it){
        ui->boxStudent->addItem(*it);
    }


/// LIST OF STUDENTS ///

    query.exec("SELECT students_info.Фамилия FROM students_info");
    int i = 1;
    while (query.next()){
        QString result = QString::number(i) + ":  " + query.value(0).toString();
        ++i;
        ui->showClass1->addItem(result);
    }

}


void ARM::on_UserInputLine_textChanged(const QString& line){
    ui->showEmail->clear();
    ui->textEmail->clear();
    QSqlQuery query = QSqlQuery(db);
    if (!query.exec("SELECT * FROM students_info")){
        qDebug() << query.lastError().text();
        return;
    }
    //qDebug() << line;
    while (query.next()){
        if (query.value(1).toString() == line){
            ui->showEmail->setStyleSheet("color: rgb(255, 255, 255)");
            ui->showEmail->setText(query.value(3).toString());
        }
    }
}

void ARM::on_btnSendEmail_clicked()
{
    QDate dateToday = QDate::currentDate();
    QTime timeToday = QTime::currentTime();
    QString premessage =  ui->showEmail->text() + ",  " + timeToday.toString("hh:mm:ss") + ",  " + dateToday.toString("dd/MM/yyyy") + "\n>\t";
    QString message = ui->textEmail->toPlainText();
    QFile textFile;
    QTextStream textStream;
    textFile.setFileName("C:\\Qt\\projects\\ARM\\archive.txt");
    if (!textFile.open(QIODevice::Append|QIODevice::ReadOnly|QIODevice::Text)){
        qDebug() << "ERROR: cannot open the file";
        return;
    }
    textStream.setDevice(&textFile);
    //QTextStream cout(stdout);
    //qDebug() << message;
    textStream << premessage << message << "\n\n\n";


    ui->textEmail->clear();
}

ARM::~ARM() { delete ui; }

void ARM::on_btnSkipped_clicked()
{
//    QSqlQuery query = QSqlQuery(db);

//    if (!query.exec("SELECT students_info.Фамилия, skipped_classes.Понедельник FROM students_info, skipped_classes WHERE students_info.Код = skipped_classes.Код")){
//        qDebug() << query.lastError().text();
//        return;
//    }
    modelSkipped = new QSqlTableModel(this, db);
    modelSkipped->setTable("skipped_classes");
    modelSkipped->removeColumn(0);
    modelSkipped->select();
    ui->tableSkipped->setModel(modelSkipped);
}

int DaysBeforeBirthday(QString date) {
    QDate dateToday = QDate::currentDate();

    QString pupDay = date.mid(0,2), pupMonth = date.mid(3,2);
    int pupilDay = pupDay.toInt(), pupilMonth = pupMonth.toInt();
    int curDay = dateToday.day(), curMonth = dateToday.month();

        //the exact amount of days doesn't matter, as we only need to estimate which birthday is closer
    int gwa = (pupilMonth - curMonth) * 30 + pupilDay - curDay;
    if (gwa < 0) gwa += 365;
    //qDebug() << date << ": " << gwa << "\n";
    return gwa;
}

void ARM::on_btnBirthday_clicked() {
    ui->viewBirthday->clear();
    QDate dateToday = QDate::currentDate();
    QSqlQuery query = QSqlQuery(db);
    if (!query.exec("SELECT * FROM students_info")){
        qDebug() << query.lastError().text();
        return;
    }
   qDebug() << "Current date: " << dateToday.toString("dd/MM/yy");
   QString minimum; int mini = 400;
   while (query.next()){
        QString birthday = query.value(2).toString();
        int currentMin = DaysBeforeBirthday(birthday);
        if (currentMin < mini){
            mini = currentMin;
            minimum = birthday;
            //qDebug() << currentMin << "  " << birthday << "\n";
        }
   }
   QString birthday = minimum.mid(0,5);
   ui->viewBirthday->insertPlainText(birthday);

}


void ARM::on_boxMarks_currentIndexChanged(const QString &arg1)
{
    ui->listMarks->clear();
     int i = 0;
    QSqlQuery query = QSqlQuery(db);
    QVector<QPair<QString, float>> vec;
    if (arg1 == "Английский"){
        query.exec("SELECT students_info.Фамилия, marks.Английский FROM students_info, marks WHERE students_info.Код = marks.Код");
    }
    if (arg1 == "Русский"){
        query.exec("SELECT students_info.Фамилия, marks.Русский FROM students_info, marks WHERE students_info.Код = marks.Код");
    }
    if (arg1 == "Математика"){
        query.exec("SELECT students_info.Фамилия, marks.Математика FROM students_info, marks WHERE students_info.Код = marks.Код");
    }
    if (arg1 == "Литература"){
        query.exec("SELECT students_info.Фамилия, marks.Литература FROM students_info, marks WHERE students_info.Код = marks.Код");
    }

    while (query.next()){
        QPair<QString, float> res1 = qMakePair(query.value(0).toString(), query.value(1).toFloat());
        vec.push_back(res1); ++i;
    }

    std::sort(vec.begin(), vec.end(),[](QPair<QString, float> a, QPair<QString, float>b){ return a.second > b.second; });
    i = 1;
    for (auto it = vec.begin(); it != vec.end(); ++it){
        QString res = QString::number(i)+ ": "+ it->first + " " + QString::number(it->second);
        ui->listMarks->addItem(res);
        ++i;
    }
    return;
}


void ARM::on_boxStudent_currentIndexChanged(const QString &arg1) {
    ui->listRating->clear();
    QSqlQuery query = QSqlQuery(db);
    QVector<QString> marks; //index of element = ID
    if (!query.exec("SELECT marks.Английский, marks.Русский, marks.Математика, marks.Литература FROM marks")) qDebug() << query.lastError().text();
    while (query.next()){
        QString res = "Aнглийский: " + query.value(0).toString() + "\nPусский: " + query.value(1).toString() + "\nMатематика: " + query.value(2).toString() + "\nЛитература: "+ query.value(3).toString();
        marks.push_back(res);
    }
    int i = 0;
    for (auto it = names.begin(); it != names.end(); ++it){
        if (*it == arg1){
            ui->listRating->addItem(marks[i]);
        }
        else ++i;
    }
}


void ARM::on_btnMeeting_clicked() {
   ui->showMeeting->clear();
   QString message = ui->getMeeting->text();
   //qDebug() << message;
   bool flag = true;
   for (auto it = meet.begin(); it != meet.end();){
       if (message == *it){
           meet.erase(it);
           it = meet.end();
           flag = false;
       }
       else ++it;
       //qDebug() << message << " " << *it;
   }
   if (flag) meet.push_back(message);
   for (auto it = meet.begin(); it != meet.end(); ++it){
       ui->showMeeting->addItem(*it);
   }
}


void ARM::on_btnUpdateClass_clicked()
{
    ui->showClass1->clear();
    QSqlQuery query;
    query.exec("SELECT students_info.Фамилия FROM students_info");
    int i = 1;
    while (query.next()){
        QString result = QString::number(i) + ":  " + query.value(0).toString();
        ++i;
        ui->showClass1->addItem(result);
    }
}

