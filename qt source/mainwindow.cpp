#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "datamodule.h"
#include <QMessageBox>
#include <QInputDialog>
#include <QDebug>
#include "dialogclassedit.h"
#include "dialogstudedit.h"
#include "dialogcreatenewclass.h"
#include <QDate>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

   if (!DM.Connect()) // Загружаем бд
       QMessageBox::about(this,"Ошибка подключения к базе",DM.getErrMsg()) ;

   ui->tabWidget->setCurrentIndex(0) ;
   // Создаем таблицы на вкладках главной формы
   reserves2table() ;
   classes2table() ;
   students2table() ;
   languages2table() ;
   levels2table() ;
   teacherlevels2table() ;
}

MainWindow::~MainWindow()
{
    delete ui;
}

// Таблица Записи на курсы
void MainWindow::reserves2table(int id)
{
    QTableWidget * table = ui->tableReserves ;

    table->setSelectionBehavior(QAbstractItemView::SelectRows) ;
    table->setSelectionMode(QAbstractItemView::SingleSelection) ;

    table->setColumnCount(7) ; // кол-во столбцов
    // Далее создаем колонки и задаем им заголовки
    table->setHorizontalHeaderItem(0,new QTableWidgetItem("id")) ;
    table->setHorizontalHeaderItem(1,new QTableWidgetItem("Курс")) ;
    table->setHorizontalHeaderItem(2,new QTableWidgetItem("Учащийся")) ;
    table->setHorizontalHeaderItem(3,new QTableWidgetItem("Цена занятия")) ;
    table->setHorizontalHeaderItem(4,new QTableWidgetItem("Число занятий")) ;
    table->setHorizontalHeaderItem(5,new QTableWidgetItem("Дата начала")) ;
    table->setHorizontalHeaderItem(6,new QTableWidgetItem("Стоимость курса")) ;

    table->setRowCount(0); // кол-во строк

    int k = 0 ;
    int sum = 0 ;
    // Создать запрос, объединяющий две таблицы students и classes  в таблицу reserves, по совпадающим
    // условиям соединения s.id=r.student_id и c.id=r.class_id
    QSqlQuery query(QString("SELECT r.id,c.classname,s.sfamily,s.sname,s.sparent,c.price,r.kurscount,r.startdate FROM reserves r ")+
                    QString("LEFT JOIN students s on s.id=r.student_id ")+
                    QString("LEFT JOIN classes c on c.id=r.class_id "));
    while (query.next()) { // пока есть записи
        table->setRowCount(k+1); // новая строка

        table->setVerticalHeaderItem(k,new QTableWidgetItem("")) ;
        // Заполняем строку таблицы Записи на курсы данными полученными после запроса
        // Каждый столбец текущей строки заполняется нужным значением, плученным от запроса, например,
        // query.value("sfamily") возвращает фамилию.
        table->setItem(k,0,new QTableWidgetItem(query.value("id").toString())) ;
        table->setItem(k,1,new QTableWidgetItem(query.value("classname").toString())) ;
        table->setItem(k,2,new QTableWidgetItem(query.value("sfamily").toString()+
                                                " "+query.value("sname").toString()+
                                                " "+query.value("sparent").toString())) ;
        table->setItem(k,3,new QTableWidgetItem(query.value("price").toString())) ;
        table->setItem(k,4,new QTableWidgetItem(query.value("kurscount").toString())) ;
        table->setItem(k,5,new QTableWidgetItem(query.value("startdate").toString())) ;
        table->setItem(k,6,new QTableWidgetItem(QString::number(query.value("kurscount").toInt()*
                                                query.value("price").toInt()))) ;
        if (query.value("id").toInt()==id) table->selectRow(k);
        k++ ;
        // подсчет стоимости курса как произведение стоимости занятия и кол-ва занятий
        sum+=query.value("kurscount").toInt()*query.value("price").toInt() ;
    }

    table->resizeColumnsToContents();
    if (id==0) table->selectRow(0);

    ui->labResInfo->setText("Всего записей: "+QString::number(k)+" на сумму: "+QString::number(sum)) ;
}

// Заполнение таблицы курсы
void MainWindow::classes2table(int id)
{
    QTableWidget * table = ui->tableClasses ;

    table->setSelectionBehavior(QAbstractItemView::SelectRows) ;
    table->setSelectionMode(QAbstractItemView::SingleSelection) ;

    table->setColumnCount(6) ; // установить кол-во столбцов
    // Далее создаем колонки и задаем им заголовки
    table->setHorizontalHeaderItem(0,new QTableWidgetItem("id")) ;
    table->setHorizontalHeaderItem(1,new QTableWidgetItem("Курс")) ;
    table->setHorizontalHeaderItem(2,new QTableWidgetItem("Язык")) ;
    table->setHorizontalHeaderItem(3,new QTableWidgetItem("Уровень курса")) ;
    table->setHorizontalHeaderItem(4,new QTableWidgetItem("Уровень преподавателя")) ;
    table->setHorizontalHeaderItem(5,new QTableWidgetItem("Цена занятия")) ;

    table->setRowCount(0);

    int k = 0 ;
    // Создать запрос, объединяющий три таблицы languages, levels, teacherlevels  в таблицу classes, по совпадающим
    // условиям соединения lt.id=c.teacherlevel_id и ll.id=c.level_id и l.id=c.language_id
    QSqlQuery query(QString("SELECT c.id,c.classname,l.language,ll.levelname,lt.levelname as tlevel,c.price FROM classes c ")+
                    QString("LEFT JOIN languages l on l.id=c.language_id ")+
                    QString("LEFT JOIN levels ll on ll.id=c.level_id ")+
                    QString("LEFT JOIN teacherlevels lt on lt.id=c.teacherlevel_id "));
    while (query.next()) { // пока есть записи
        table->setRowCount(k+1); // новая строка

        table->setVerticalHeaderItem(k,new QTableWidgetItem("")) ;
        // Заполняем строку таблицы Курсы  данными полученными после запроса
        // Каждый столбец текущей строки заполняется нужным значением, плученным от запроса, например,
        // query.value("classname") возвращает название курса.
        table->setItem(k,0,new QTableWidgetItem(query.value("id").toString())) ;
        table->setItem(k,1,new QTableWidgetItem(query.value("classname").toString())) ;
        table->setItem(k,2,new QTableWidgetItem(query.value("language").toString())) ;
        table->setItem(k,3,new QTableWidgetItem(query.value("levelname").toString())) ;
        table->setItem(k,4,new QTableWidgetItem(query.value("tlevel").toString())) ;
        table->setItem(k,5,new QTableWidgetItem(query.value("price").toString())) ;
        if (query.value("id").toInt()==id) table->selectRow(k);
        k++ ;
    }

    table->resizeColumnsToContents();
    if (id==0) table->selectRow(0);

    ui->labClassInfo->setText("Всего курсов: "+QString::number(k)) ; // вывести в метке кол-во курсов
}

// Заполнение таблицы Обучающиеся
void MainWindow::students2table(int id)
{
    QTableWidget * table = ui->tableStudents ;

    table->setSelectionBehavior(QAbstractItemView::SelectRows) ;
    table->setSelectionMode(QAbstractItemView::SingleSelection) ;

    table->setColumnCount(4) ; // кол-во столбцов
    // Далее создаем колонки и задаем им заголовки
    table->setHorizontalHeaderItem(0,new QTableWidgetItem("id")) ;
    table->setHorizontalHeaderItem(1,new QTableWidgetItem("ФИО")) ;
    table->setHorizontalHeaderItem(2,new QTableWidgetItem("Документ")) ;
    table->setHorizontalHeaderItem(3,new QTableWidgetItem("Телефон")) ;

    table->setRowCount(0);

    int k = 0 ;
    // Запрос на получение всех записей из таблицы бд students отсортированной по параметрам (после ORDER BY)
    QSqlQuery query(QString("SELECT * FROM students ORDER BY sfamily,sname,sparent ")) ;
    while (query.next()) { // пока есть записи
        table->setRowCount(k+1); // новая строка

        table->setVerticalHeaderItem(k,new QTableWidgetItem("")) ;
        // Заполняем столбцы строки, записывая в каждый соответствущий параметр запроса, например
        // query.value("phone") запишет в столбец номер телефона
        table->setItem(k,0,new QTableWidgetItem(query.value("id").toString())) ;
        table->setItem(k,1,new QTableWidgetItem(query.value("sfamily").toString()+" "+
                                                query.value("sname").toString()+" "+
                                                query.value("sparent").toString())) ;
        table->setItem(k,2,new QTableWidgetItem(query.value("document").toString())) ;
        table->setItem(k,3,new QTableWidgetItem(query.value("phone").toString())) ;
        if (query.value("id").toInt()==id) table->selectRow(k);
        k++ ;
    }

    table->resizeColumnsToContents();
    if (id==0) table->selectRow(0);

    ui->labStudInfo->setText("Всего обучающихся: "+QString::number(k)) ;
}

// Заполнение таблицы Языки
void MainWindow::languages2table(int id)
{
    QTableWidget * table = ui->tableLang ;

    table->setSelectionBehavior(QAbstractItemView::SelectRows) ;
    table->setSelectionMode(QAbstractItemView::SingleSelection) ;

    table->setColumnCount(2) ; // кол-во столбцов
    // Далее создаем колонки и задаем им заголовки
    table->setHorizontalHeaderItem(0,new QTableWidgetItem("id")) ;
    table->setHorizontalHeaderItem(1,new QTableWidgetItem("Язык")) ;

    table->setRowCount(0);

    int k = 0 ;

    // Запрос на получение всех записей из таблицы бд languages
    QSqlQuery query(QString("SELECT * FROM languages "));
    while (query.next()) {
        table->setRowCount(k+1);

        table->setVerticalHeaderItem(k,new QTableWidgetItem("")) ;
        // Заполняем столбцы строки, записывая в каждый соответствущий параметр запроса, например
        // query.value("language") запишет в столбец номер язык
        table->setItem(k,0,new QTableWidgetItem(query.value("id").toString())) ;
        table->setItem(k,1,new QTableWidgetItem(query.value("language").toString())) ;
        if (query.value("id").toInt()==id) table->selectRow(k);
        k++ ;
    }

    table->resizeColumnsToContents();
    if (id==0) table->selectRow(0);

    ui->labLangInfo->setText("Всего языков: "+QString::number(k)) ;
}

// Заполнение таблицы Уровни
void MainWindow::levels2table(int id)
{
    QTableWidget * table = ui->tableLevel ;

    table->setSelectionBehavior(QAbstractItemView::SelectRows) ;
    table->setSelectionMode(QAbstractItemView::SingleSelection) ;

    table->setColumnCount(2) ; // кол-во столбцов
    // Далее создаем колонки и задаем им заголовки
    table->setHorizontalHeaderItem(0,new QTableWidgetItem("id")) ;
    table->setHorizontalHeaderItem(1,new QTableWidgetItem("Уровень")) ;

    table->setRowCount(0);

    int k = 0 ;
    // Запрос на получение всех записей из таблицы бд levels
    QSqlQuery query(QString("SELECT * FROM levels"));
    while (query.next()) {
        table->setRowCount(k+1);

        table->setVerticalHeaderItem(k,new QTableWidgetItem("")) ;
        // Заполняем столбцы строки, записывая в каждый соответствущий параметр запроса, например
        // query.value("levelname") запишет в столбец уровень
        table->setItem(k,0,new QTableWidgetItem(query.value("id").toString())) ;
        table->setItem(k,1,new QTableWidgetItem(query.value("levelname").toString())) ;
        if (query.value("id").toInt()==id) table->selectRow(k);
        k++ ;
    }

    table->resizeColumnsToContents();
    if (id==0) table->selectRow(0);

    ui->labLevelInfo->setText("Всего уровней: "+QString::number(k)) ;
}

// Заполнение таблицы Уровни Преподавателей
void MainWindow::teacherlevels2table(int id)
{
    QTableWidget * table = ui->tableTLevel ;

    table->setSelectionBehavior(QAbstractItemView::SelectRows) ;
    table->setSelectionMode(QAbstractItemView::SingleSelection) ;

    table->setColumnCount(2) ; // кол-во столбцов
       // Далее создаем колонки и задаем им заголовки
    table->setHorizontalHeaderItem(0,new QTableWidgetItem("id")) ;
    table->setHorizontalHeaderItem(1,new QTableWidgetItem("Уровень")) ;

    table->setRowCount(0);

    int k = 0 ;
    // Запрос на получение всех записей из таблицы бд teacherlevels
    QSqlQuery query(QString("SELECT * FROM teacherlevels"));
    while (query.next()) {
        table->setRowCount(k+1);

        table->setVerticalHeaderItem(k,new QTableWidgetItem("")) ;
        // Заполняем столбцы строки, записывая в каждый соответствущий параметр запроса, например
        // query.value("levelname") запишет в столбец уровень преподавателя
        table->setItem(k,0,new QTableWidgetItem(query.value("id").toString())) ;
        table->setItem(k,1,new QTableWidgetItem(query.value("levelname").toString())) ;
        if (query.value("id").toInt()==id) table->selectRow(k);
        k++ ;
    }

    table->resizeColumnsToContents();
    if (id==0) table->selectRow(0);

    ui->labTLevelInfo->setText("Всего уровней: "+QString::number(k)) ;
}

// Слот после нажатия Новый Курс
void MainWindow::on_butAddClass_clicked()
{
    DialogClassEdit d(this) ; // создать диалог
    d.newClass() ;
    d.exec() ; // запустить в модальном режиме
    if (d.isUpdated()) // если обновили то вызываем функцию
        classes2table() ;
}

// Слот после нажатия Корректировать Курс
void MainWindow::on_butEditClass_clicked()
{
    if (ui->tableClasses->currentRow()==-1) {
        QMessageBox::information(this,"Ошибка","Не выбран курс для коррекции") ;
        return ;
    }

    // получить id в в выбранной строке таблице на форме
    int id = ui->tableClasses->item(ui->tableClasses->currentRow(),0)->text().toInt() ;

    DialogClassEdit d(this) ; // диалог редактирования
    d.editClass(id) ;
    d.exec() ;// запустить в модальном режиме
    if (d.isUpdated()) {  // если обновили то вызываем функции
        classes2table(id) ;
        reserves2table();
    }
}

// Слот после нажатия Удалить Курс
void MainWindow::on_butDelClass_clicked()
{
    int curRow = ui->tableClasses->currentRow();
     // получить id в выбранной строке таблице на форме
    QString id = ui->tableClasses->item(curRow, 0)->data(0).toString();
    if (DM.deleteClassById(id)) { // вызов удаления из базы и если успешно
        ui->tableClasses->removeRow(curRow); // удалить строку из таблицы на форме
        reserves2table(); // обновить таблицу Записи на курсы
    }
}

// Слот после нажатия Записать на Курс
void MainWindow::on_butAddToClass_clicked()
{
    DialogCreateNewClass d(this); // открыть диалог новой записи
    d.setWindowTitle("Добавить занятие");
    d.exec();

    if (d.isUpdated()) {
        int idClass;
        QSqlQuery query ;

        // получаем id Курса
        if (query.exec("SELECT id, classname FROM classes")) {
            while (query.next()) {
                if (query.value("classname") == d.getClassName()) {
                    idClass = query.value("id").toInt();
                    break;
                }
            }
        }

        // Подготовка на запрос на вставку данных новой записи на курс
        query.prepare(QString("INSERT INTO reserves (student_id,class_id,price,kurscount,startdate) ")+
                      QString("VALUES (:st_id, :class_id, :price,:kurscount, :startdate)")) ;

        int curRow = ui->tableStudents->currentRow();

        // Связываем данные с данными введенными в диалоге создания новой записи и  id курса
        query.bindValue(":st_id", ui->tableStudents->item(curRow, 0)->data(0).toInt());
        query.bindValue(":class_id", idClass);
        query.bindValue(":price", ui->tableClasses->item(curRow, 5)->data(0).toInt());
        query.bindValue(":kurscount", d.getCountLessons());
        query.bindValue(":startdate", d.getDate());

        if (query.exec()) // выполнить запрос
            reserves2table();
    }
}

// Слот после нажатия Новый Обучающийся
void MainWindow::on_butAddStud_clicked()
{
    DialogStudEdit d(this) ; // запуск диалога
    d.newStud() ;
    d.exec() ;// запустить в модальном режиме
    if (d.isUpdated()) // если обновили в диалоге то оновить таблицы
        students2table() ;
}

// Слот после нажатия Корректировать Обучающийся
void MainWindow::on_butEditStud_clicked()
{
    if (ui->tableStudents->currentRow()==-1) {
        QMessageBox::information(this,"Ошибка","Не выбран обучающийся для коррекции") ;
        return ;
    }

    // получить id в выбранной строке таблице на форме
    int id = ui->tableStudents->item(ui->tableStudents->currentRow(),0)->text().toInt() ;

    DialogStudEdit d(this) ; // запуск диалога
    d.editStud(id) ;
    d.exec() ;// запустить в модальном режиме
    if (d.isUpdated()) { // если обновили в диалоге то оновить таблицы
        students2table(id) ;
        reserves2table();
    }
}

// Слот после нажатия Удалить Обучающийся
void MainWindow::on_butDelStud_clicked()
{
    int curRow = ui->tableStudents->currentRow();
    auto item = ui->tableStudents->item(curRow, 0);
    QString id = item->data(0).toString(); // получить id в выбранной строке таблице на форме
    if (DM.deleteStudentById(id)) { // удалить из бд таблицы students по id
        ui->tableStudents->removeRow(curRow); // удалить строку из таблицы на форме
        reserves2table(); // обновить таблицу записей на курсы
    }
}

// Слот после нажатия Новый Язык
void MainWindow::on_butAddLang_clicked()
{
    // получить название языка
    QString str=QInputDialog::getText(this,"Новый язык","Введите язык") ;
    if (str.length()>0) {
        DM.addLanguage(str) ;// добавить в бд
        languages2table() ; // обновить таблицу на форме
    }
}

// Слот после нажатия Редактировать  Язык
void MainWindow::on_butEditLang_clicked()
{
    // получить id в выбранной строке таблице на форме
    int id = ui->tableLang->item(ui->tableLang->currentRow(),0)->text().toInt() ;
    bool ok = false;
    // получить новое название языка
    QString str=QInputDialog::getText(this,"Изменить язык","Введите язык", QLineEdit::Normal,"", &ok) ;
    if (ok && str.length()>0) {
        QSqlQuery query ;
        if (id==0) {
            query.prepare(QString("INSERT INTO languages (language) VALUES (:v)")) ;
        }
        else { // обновляем название языка по его id
            query.prepare(QString("UPDATE languages SET language=:v WHERE id=:id"));
            query.bindValue(":id",id);
        }
        query.bindValue(":v",str);

        if (!query.exec()) {
            QMessageBox::information(this,"Ошибка","Ошибка редактирования языка") ;
            return;
        }

        // обновляем таблицы
        languages2table();
        classes2table();
    }
}

// Слот после нажатия Удалить  Язык
void MainWindow::on_butDelLang_clicked()
{
    int curRow = ui->tableLang->currentRow();
    auto item = ui->tableLang->item(curRow, 0);
    QString id = item->data(0).toString(); // получить id в выбранной строке таблице на форме
    if (DM.deleteLangById(id)) { // удаляем из бд таблицы languages
        ui->tableLang->removeRow(curRow); // удалить из таблицы на форме
        classes2table(); // обновить таблицу курсов
    }
}

// Слот после нажатия Новый Уровень курсов
void MainWindow::on_butAddLevel_clicked()
{
    // новое название для добавления
    QString str=QInputDialog::getText(this,"Новый уровень","Введите уровень") ;
    if (str.length()>0) {
        DM.addLevel(str) ; // добавить в базу
        levels2table() ; // обновить таблицу
    }
}

// Слот после нажатия Редактировать Уровень курсов
void MainWindow::on_butEditLevel_clicked()
{
    // получить id в выбранной строке таблице на форме
    int id = ui->tableLevel->item(ui->tableLevel->currentRow(),0)->text().toInt() ;
    bool ok = false;
    // новое название для редактирования
    QString str=QInputDialog::getText(this,"Изменить уровень","Введите уровень", QLineEdit::Normal,"", &ok) ;

    if (ok && str.length()>0) {
        QSqlQuery query ;
        if (id==0) {
            query.prepare(QString("INSERT INTO levels (levelname) VALUES (:v)")) ;
        }
        else { // обновить имя уровня в таблице levels бд
            query.prepare(QString("UPDATE levels SET levelname=:v WHERE id=:id"));
            query.bindValue(":id",id);
        }
        query.bindValue(":v",str);

        if (!query.exec()) {
            QMessageBox::information(this,"Ошибка","Ошибка редактирования уровня") ;
            return;
        }

        // обновить таблицы
        levels2table();
        classes2table();
    }
}

// Слот после нажатия Удалить Уровень курсов
void MainWindow::on_butDelLevel_clicked()
{
    int curRow = ui->tableLevel->currentRow();
    auto item = ui->tableLevel->item(curRow, 0);
    QString id = item->data(0).toString(); // получить id
    if (DM.deleteLevelById(id)) { // удалить из бд таблицы levels
        ui->tableLevel->removeRow(curRow); // удалить строку из таблиц ына форме
        classes2table(); // обновить
    }
}

// Слот после нажатия Новый Уровень преподавателей
void MainWindow::on_butAddTLevel_clicked()
{
    // получить название
    QString str=QInputDialog::getText(this,"Новый уровень","Введите уровень") ;
    if (str.length()>0) {
        DM.addTeacherLevel(str) ; // добавить в бд в таблицу teacherlevels
        teacherlevels2table() ; // обновить таблицу формы
    }
}

// Слот после нажатия Редактировать Уровень преподавателей
void MainWindow::on_butEditTLevel_clicked()
{
    // получить Id
    int id = ui->tableTLevel->item(ui->tableTLevel->currentRow(),0)->text().toInt() ;
    bool ok = false;
    QString str = QInputDialog::getText(this,"Изменить уровень преподавателя","Введите уровень", QLineEdit::Normal,"", &ok) ;

    if (ok && str.length()>0) {
        QSqlQuery query ;
        if (id == 0) {
            query.prepare(QString("INSERT INTO teacherlevels (levelname) VALUES (:v)")) ;
        }
        else { // обновить имя уровня в таблице teacherlevels по id
            query.prepare(QString("UPDATE teacherlevels SET levelname=:v WHERE id=:id"));
            query.bindValue(":id",id);
        }
        query.bindValue(":v",str);

        if (!query.exec()) { // выполнить запрос
            QMessageBox::information(this,"Ошибка","Ошибка редактирования уровня преподавателя") ;
            return;
        }

        // обновить таблицы
        teacherlevels2table();
        classes2table();
    }
}

// Слот после нажатия Удалить Уровень преподавателей
void MainWindow::on_butDelTLevel_clicked()
{
    int curRow = ui->tableTLevel->currentRow();
    auto item = ui->tableTLevel->item(curRow, 0);
    QString id = item->data(0).toString();
    if (DM.deleteTLevelById(id)) { // удалить из бд таблицы
        ui->tableTLevel->removeRow(curRow); // удалить строку из таблицы на форме
        teacherlevels2table(); // обновить таблицы
        classes2table();
    }
}

// Слот после нажатия Редактировать Запись на курс
void MainWindow::on_butEditRes_clicked()
{
    DialogCreateNewClass d(this);
    d.setWindowTitle("Редактирование занятия");
    d.exec();

    if (d.isUpdated()) {
        int idClass;

        // поиск id курса
        QSqlQuery query ;
        if (query.exec("SELECT id, classname FROM classes")) {
            while (query.next()) {
                if (query.value("classname") == d.getClassName()) {
                    idClass = query.value("id").toInt();
                    break;
                }
            }
        }

        int curRow = ui->tableReserves->currentRow();
        auto item = ui->tableReserves->item(curRow, 0);
        int id = item->data(0).toInt(); //  id записи

        // подготовка запроса на обновление записей в таблице reserves бд
        query.prepare(QString("UPDATE reserves SET class_id:=class_id, kurscount:=kurscount,startdate:=startdate")+
                      QString("WHERE id=:id")) ;

        query.bindValue(":id",QString::number(id)) ;
        query.bindValue(":class_id", QString::number(idClass));
        query.bindValue(":kurscount", QString::number(d.getCountLessons()));
        query.bindValue(":startdate", d.getDate());

        if (query.exec()) { // выполнить запрос
            reserves2table(); // обновить таблицу
        }
    }
}

// Слот после нажатия Удалить Запись на курс
void MainWindow::on_butDelRes_clicked()
{
    int curRow = ui->tableReserves->currentRow();
    auto item = ui->tableReserves->item(curRow, 0);
    QString id = item->data(0).toString(); // получить id для удаления
    if (DM.deleteReservesById(id)) { // удалить из бд таблицы reserves
        ui->tableReserves->removeRow(curRow); // удалить строку из таблицы на форме
        reserves2table(); // обновить таблицу
    }
}
