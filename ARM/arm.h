#ifndef ARM_H
#define ARM_H

#include <QMainWindow>
#include <QWidget>
//#include <QTableView>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlTableModel>

QT_BEGIN_NAMESPACE
namespace Ui { class ARM; }
QT_END_NAMESPACE

class ARM : public QMainWindow
{
    Q_OBJECT

public:
    ARM(QWidget *parent = nullptr);
    ~ARM();

signals:

public slots:

private slots:
    void on_UserInputLine_textChanged(const QString& line);

    void on_btnSkipped_clicked();

    void on_btnBirthday_clicked();

    void on_btnSendEmail_clicked();

    void on_boxMarks_currentIndexChanged(const QString &arg1);

    void on_boxStudent_currentIndexChanged(const QString &arg1);

    void on_btnMeeting_clicked();

    void on_btnUpdateClass_clicked();

private:
    Ui::ARM *ui;
    QSqlDatabase db;
    QSqlTableModel* modelSkipped;
    QVector<QString> names;
    QVector<QString> meet {};

};
#endif // ARM_H
