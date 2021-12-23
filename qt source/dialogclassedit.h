#ifndef DIALOGCLASSEDIT_H
#define DIALOGCLASSEDIT_H

#include <QDialog>

namespace Ui {
class DialogClassEdit;
}

// Класс формы диалогового окна по редактированию Курса
class DialogClassEdit : public QDialog
{
    Q_OBJECT

public:
    explicit DialogClassEdit(QWidget *parent = nullptr);
    ~DialogClassEdit();

    void newClass() ;
    void editClass(int id) ;
    bool isUpdated() const ;

private:
    Ui::DialogClassEdit *ui;
    int id ;
    bool isupdated ; // флаг было обновление или нет

private slots:
    void save() ;
};

#endif // DIALOGCLASSEDIT_H
