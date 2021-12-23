#include "dialogclassedit.h"
#include "ui_dialogclassedit.h"
#include "datamodule.h"
#include <QMessageBox>
#include <QPushButton>
#include <QComboBox>

// Конструктор
DialogClassEdit::DialogClassEdit(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogClassEdit)
{
    ui->setupUi(this);
    isupdated = false ;
    id = 0 ;

    auto langs = DM.getLanguages() ; // загружаем словарь таблицы языков
    foreach (auto key, langs.keys()) // заполняем всплывающий список на форме данными словаря языков (перечень языков)
        ui->comboLanguage->addItem(langs[key],key);

    auto levels = DM.getLevels() ; // загружаем словарь таблицы уровней
    foreach (auto key, levels.keys()) // заполняем всплывающий список на форме данными уровней (перечень уровней)
        ui->comboLevel->addItem(levels[key],key);

    auto teacherlevels = DM.getTeacherLevels() ; // загружаем словарь таблицы уровней преподавателей
    foreach (auto key, teacherlevels.keys()) // заполняем соответствующий всплывающий список на форме
        ui->comboTeacherLevel->addItem(teacherlevels[key],key);

    ui->buttonBox->button(QDialogButtonBox::Ok)->setText("ОК");
    ui->buttonBox->button(QDialogButtonBox::Cancel)->setText("Отмена");

    ui->buttonBox->disconnect(SIGNAL(accepted())) ;
    connect(ui->buttonBox,SIGNAL(accepted()),this,SLOT(save())) ;  // сигнал-слот
}

// Деструктор
DialogClassEdit::~DialogClassEdit()
{
    delete ui;
}

// Создать новый курс
void DialogClassEdit::newClass()
{
    this->id=0 ;
    // установить индексы всплывающих списков
    ui->comboLanguage->setCurrentIndex(-1) ;
    ui->comboLevel->setCurrentIndex(-1) ;
    ui->comboTeacherLevel->setCurrentIndex(-1) ;
}

// Функция заполняет поля и всплывающие списки данными текущего объекта ( курса)
void DialogClassEdit::editClass(int id)
{
    this->id = id;

    // Выбрать все поля из таблицы classes по id
    QSqlQuery query("SELECT * FROM classes WHERE id="+QString::number(id));
    if (query.next()) { // пока есть записи
        // устанавливаем данные виджетов
        ui->edClassName->setText(query.value("classname").toString());
        ui->edPrice->setValue(query.value("price").toInt());
        int index = ui->comboLanguage->findData(query.value("language_id").toInt()) ;
        if (index!=-1) ui->comboLanguage->setCurrentIndex(index);
        index = ui->comboLevel->findData(query.value("level_id").toInt()) ;
        if (index!=-1) ui->comboLevel->setCurrentIndex(index);
        index = ui->comboTeacherLevel->findData(query.value("teacherlevel_id").toInt()) ;
        if (index!=-1) ui->comboTeacherLevel->setCurrentIndex(index);
    }
}

// Возврат было ли обновление
bool DialogClassEdit::isUpdated() const
{
    return isupdated ;
}

// Сохранить изменения по нажатию кнопки OK
void DialogClassEdit::save()
{
    // Проверка что было выбрано значение
    if (ui->comboLanguage->currentIndex()==-1) {
        QMessageBox::information(this,"Ошибка","Не указан язык") ;
        return ;
    }

    // Проверка что было выбрано значение
    if (ui->comboLevel->currentIndex()==-1) {
        QMessageBox::information(this,"Ошибка","Не указан уровень") ;
        return ;
    }

    // Проверка что было выбрано значение
    if (ui->comboTeacherLevel->currentIndex()==-1) {
        QMessageBox::information(this,"Ошибка","Не указан уровень преподавателя") ;
        return ;
    }

    // Проверка что было выбрано значение
    if (ui->edClassName->text().length()==0) {
        QMessageBox::information(this,"Ошибка","Не указано название курса") ;
        return ;
    }

    QSqlQuery query ;
    if (id==0) { // если не установлен id то вставка
        query.prepare(QString("INSERT INTO classes (classname,level_id,teacherlevel_id,language_id,price) ")+
                      QString("VALUES (:classname,:level_id,:teacherlevel_id,:language_id,:price)")) ;
    }
    else { // иначе подготавливаем запрос для обновления полей таблицы classes данными
        query.prepare(QString("UPDATE classes SET classname=:classname,")+
                      QString("level_id=:level_id,teacherlevel_id=:teacherlevel_id,language_id=:language_id,")+
                      QString("price=:price ")+
                      QString("WHERE id=:id")) ;
        query.bindValue(":id",id) ;
    }

    // связываем данные имени курса
    query.bindValue(":classname",ui->edClassName->text());

    // связываем данные уровней
    auto levels = DM.getLevels();
    int id = levels.key(ui->comboLevel->currentText());
    query.bindValue(":level_id", id);

    // // связываем данные уровней преподавателей
    auto levelsT = DM.getTeacherLevels();
    id = levelsT.key(ui->comboTeacherLevel->currentText());
    query.bindValue(":teacherlevel_id",id);

    // связываем данные языков
    auto langs = DM.getLanguages() ;
    id = langs.key(ui->comboLanguage->currentText());
    query.bindValue(":language_id", id);

    // связываем данные стоимости
    query.bindValue(":price",ui->edPrice->value()) ;

    if (!query.exec()) // выполняем запрос и проверяем его успешность
        QMessageBox::information(this,"Ошибка","Ошибка сохранения курса") ;

    isupdated=true ;

    accept() ; // закрывает диалог и возвращает accept
}
