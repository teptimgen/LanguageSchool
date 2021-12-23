#include "dialogcreatenewclass.h"
#include "ui_dialogcreatenewclass.h"
#include "datamodule.h"
#include <QPushButton>
#include <QMessageBox>

DialogCreateNewClass::DialogCreateNewClass(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogCreateNewClass)
{
    ui->setupUi(this);

    isupdated = false ;
    id = 0 ;

    // выбираем название всех курсов
    QSqlQuery query("SELECT id, classname FROM classes");
    while (query.next()) { // заполняем всплывающий список на форме
        ui->comboBoxChooseClass->addItem(query.value("classname").toString());
    }

    ui->comboBoxChooseClass->setCurrentIndex(-1);

    ui->buttonBox->button(QDialogButtonBox::Ok)->setText("ОК");
    ui->buttonBox->button(QDialogButtonBox::Cancel)->setText("Отмена");

    ui->buttonBox->disconnect(SIGNAL(accepted())) ;
    connect(ui->buttonBox,SIGNAL(accepted()),this,SLOT(save())) ;
}

DialogCreateNewClass::~DialogCreateNewClass()
{
    delete ui;
}

// Сохранить изменения по нажатию кнопки OK
void DialogCreateNewClass::save()
{
    // Проверка что было выбрано значение
    if (ui->comboBoxChooseClass->currentIndex()==-1) {
        QMessageBox::information(this,"Ошибка","Не указан курс") ;
        return ;
    }

    // Проверка что было введено значение
    if (ui->lineEditDateStart->text().length() == 0) {
        QMessageBox::information(this,"Ошибка","Не указана дата") ;
        return ;
    }

    // сохраняем введенные в диалоге данные в переменные класса
    isupdated = true;
    className = ui->comboBoxChooseClass->currentText();
    countLessons = ui->spinBoxCountClass->value();
    date = ui->lineEditDateStart->text();

    accept();
}

QString DialogCreateNewClass::getClassName() const
{
    return className;
}

void DialogCreateNewClass::setClassName(const QString &value)
{
    className = value;
}

QString DialogCreateNewClass::getDate() const
{
    return date;
}

void DialogCreateNewClass::setDate(const QString &value)
{
    date = value;
}

int DialogCreateNewClass::getCountLessons() const
{
    return countLessons;
}

void DialogCreateNewClass::setCountLessons(int value)
{
    countLessons = value;
}
