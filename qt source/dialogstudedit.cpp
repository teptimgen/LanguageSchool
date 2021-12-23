#include "dialogstudedit.h"
#include "ui_dialogstudedit.h"
#include <QPushButton>
#include "datamodule.h"
#include <QMessageBox>

// Конструктор
DialogStudEdit::DialogStudEdit(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogStudEdit)
{
    ui->setupUi(this);
    isupdated = false ;
    id = 0 ;

    ui->buttonBox->button(QDialogButtonBox::Ok)->setText("ОК");
    ui->buttonBox->button(QDialogButtonBox::Cancel)->setText("Отмена");

    ui->buttonBox->disconnect(SIGNAL(accepted())) ;
    connect(ui->buttonBox,SIGNAL(accepted()),this,SLOT(save())) ;
}

DialogStudEdit::~DialogStudEdit()
{
    delete ui;
}

void DialogStudEdit::newStud()
{
    this->id=0 ;
}

// Функция редактирования
void DialogStudEdit::editStud(int id)
{
    this->id = id ; // запоминаем id

    // запрос на получение записей из таблицы students где совпадают id
    QSqlQuery query("SELECT * FROM students WHERE id="+QString::number(id));
    if (query.next()) { // пока есть записи
        // заполняем виджеты на форме диалога данными из полученной записи
        ui->edFamily->setText(query.value("sfamily").toString());
        ui->edName->setText(query.value("sname").toString());
        ui->edParent->setText(query.value("sparent").toString());
        ui->edDocument->setText(query.value("document").toString());
        ui->edPhone->setText(query.value("phone").toString());
    }
}

bool DialogStudEdit::isUpdated() const
{
    return isupdated ;
}

void DialogStudEdit::save()
{
    // Проверка что было введено значение
    if (ui->edFamily->text().length()==0) {
        QMessageBox::information(this,"Ошибка","Не указана фамилия") ;
        return ;
    }

    // Проверка что было введено значение
    if (ui->edName->text().length()==0) {
        QMessageBox::information(this,"Ошибка","Не указано имя") ;
        return ;
    }

    // Проверка что было введено значение
    if (ui->edParent->text().length()==0) {
        QMessageBox::information(this,"Ошибка","Не указано отчество") ;
        return ;
    }

    // Проверка что было введено значение
    if (ui->edDocument->text().length()==0) {
        QMessageBox::information(this,"Ошибка","Не указан документ") ;
        return ;
    }

    // Проверка что было введено значение
    if (ui->edPhone->text().length()==0) {
        QMessageBox::information(this,"Ошибка","Не указан телефон") ;
        return ;
    }

    QSqlQuery query ;
    if (id==0) { // если не установлен id то вставка
        query.prepare(QString("INSERT INTO students (sfamily,sname,sparent,document,phone) ")+
                      QString("VALUES (:sfamily,:sname,:sparent,:document,:phone)")) ;
    }
    else { // иначе подготавливаем запрос для обновления полей таблицы students данными
        query.prepare(QString("UPDATE students SET sfamily=:sfamily,")+
                      QString("sname=:sname,sparent=:sparent,document=:document,")+
                      QString("phone=:phone ")+
                      QString("WHERE id=:id")) ;
        query.bindValue(":id",id) ;
    }
    query.bindValue(":sfamily",ui->edFamily->text()); // связываем фамилию
    query.bindValue(":sname",ui->edName->text()); // связываем имя
    query.bindValue(":sparent",ui->edParent->text()); // связываем отчество
    query.bindValue(":document",ui->edDocument->text()); // связываем паспорт
    query.bindValue(":phone",ui->edPhone->text()) ; // связываем телефон

    if (!query.exec()) // выполняем запрос и проверяем результат
        QMessageBox::information(this,"Ошибка","Ошибка сохранения обучающегося") ;

    isupdated=true ;

    accept() ; // закрывает диалог и возвращает accept
}
