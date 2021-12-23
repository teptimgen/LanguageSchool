#ifndef DIALOGSTUDEDIT_H
#define DIALOGSTUDEDIT_H

#include <QDialog>

namespace Ui {
class DialogStudEdit;
}

// Класс диалогового окна для редактирования обучающихся
class DialogStudEdit : public QDialog
{
    Q_OBJECT

public:
    explicit DialogStudEdit(QWidget *parent = nullptr);
    ~DialogStudEdit();

    void newStud() ;
    void editStud(int id) ;
    bool isUpdated() const ;

private:
    Ui::DialogStudEdit *ui;
    int id ;
    bool isupdated ;

private slots:
    void save() ;
};

#endif // DIALOGSTUDEDIT_H
