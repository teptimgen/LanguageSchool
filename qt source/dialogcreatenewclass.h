#ifndef DIALOGCREATENEWCLASS_H
#define DIALOGCREATENEWCLASS_H

#include <QDialog>
#include <QMap>

namespace Ui {
class DialogCreateNewClass;
}

// Класс диалогового окна для создания новой записи на курс
class DialogCreateNewClass : public QDialog
{
    Q_OBJECT

public:
    explicit DialogCreateNewClass(QWidget *parent = 0);
    ~DialogCreateNewClass();
    bool isUpdated() const { return isupdated; }


    // сеттер-геттер
    QString getDate() const;
    void setDate(const QString &value);

    // сеттер-геттер
    int getCountLessons() const;
    void setCountLessons(int value);

    // сеттер-геттер
    QString getClassName() const;
    void setClassName(const QString &value);

private slots:
    void save() ;

private:
    Ui::DialogCreateNewClass *ui;
    int id;
    bool isupdated ;
    QString className; // имя
    QString date; // дата
    int countLessons; // кол-во занятий
};

#endif // DIALOGCREATENEWCLASS_H
