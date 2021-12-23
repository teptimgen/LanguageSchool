#include "datamodule.h"
#include <QMessageBox>

DataModule DM ;

DataModule::DataModule()
{
}

// Функция подключения к базе данных
bool DataModule::Connect()
{
    QSqlDatabase sdb = QSqlDatabase::addDatabase("QSQLITE"); // создать объект бд
    sdb.setDatabaseName("database.db"); // установить путь к файлу бд

    if (sdb.open()) // если бд открыта
        return true ;
    else {
        ErrMsg = sdb.lastError().text();
        return false ;
    }
}

// Функция возвращает ошибку
QString DataModule::getErrMsg() const
{
    return ErrMsg ;
}

// Функция возвращает данные таблицы Языки в виде словаря, хранящем пару id-language
// и возвращает данный словарь
QMap<int, QString> DataModule::getLanguages()
{
    QMap<int,QString> res; // словарь
    // запрос выбрать id и language из таблицы languages отсортированному по language
    QSqlQuery query("SELECT id,language FROM languages ORDER BY language");
    while (query.next()) // пока есть записи после этого запроса последовательно заполняем словарь
        res[query.value("id").toInt()]=query.value(1).toString();
    return res;
}

// Функция возвращает данные таблицы Уровни в виде словаря, хранящем пару id-levelname
// и возвращает данный словарь
QMap<int, QString> DataModule::getLevels()
{
    QMap<int,QString> res ;
    // запрос выбрать id и levelname из таблицы levels отсортированному по levelname
    QSqlQuery query("SELECT id,levelname FROM levels ORDER BY levelname");
    while (query.next())
             res[query.value("id").toInt()]=query.value(1).toString() ;
    return res ;
}

// Функция возвращает данные таблицы Уровень преподавателя в виде словаря, хранящем пару id-levelname
// и возвращает данный словарь
QMap<int, QString> DataModule::getTeacherLevels()
{
    QMap<int,QString> res ;
    // запрос выбрать id и levelname из таблицы teacherlevels отсортированному по levelname
    QSqlQuery query("SELECT id,levelname FROM teacherlevels ORDER BY levelname");
    while (query.next())
             res[query.value("id").toInt()]=query.value(1).toString() ;
    return res ;
}

// Функция вставляет в таблицу Языки переданный в качестве параметра новый язык
bool DataModule::addLanguage(const QString &language)
{
    QSqlQuery query ;
    // запрос на вставку в таблицу Язык значения которое связывается с переменной :v
    query.prepare(QString("INSERT INTO languages (language) VALUES (:v)")) ;
    query.bindValue(":v",language); // связать переменную :v с переданным в функцию новым языком
    if (!query.exec()) { // если вставка не успешна
        ErrMsg = "Ошибка сохранения языка" ;
        return false ;
    }
    else
        return true ;
}

// Функция вставляет в таблицу Уровни Курсов переданный в качестве параметра новый уровень
bool DataModule::addLevel(const QString &level)
{
    QSqlQuery query ;
    // запрос на вставку в таблицу Уровни Курсов значения которое связывается с переменной :v
    query.prepare(QString("INSERT INTO levels (levelname) VALUES (:v)")) ;
    query.bindValue(":v",level); // связать переменную :v с переданным в функцию новым уровнем
    if (!query.exec()) {
        ErrMsg = "Ошибка сохранения уровня" ;
        return false ;
    }
    else
        return true ;
}

// Функция вставляет в таблицу Уровни Преподавателей переданный в качестве параметра новый уровень
bool DataModule::addTeacherLevel(const QString &level)
{
    QSqlQuery query ;
    // запрос на вставку в таблицу Уровни Преподавателей значения которое связывается с переменной :v
    query.prepare(QString("INSERT INTO teacherlevels (levelname) VALUES (:v)")) ;
    query.bindValue(":v",level); // связать переменную :v с переданным в функцию новым уровнем учителя
    if (!query.exec()) {
        ErrMsg = "Ошибка сохранения уровня" ;
        return false ;
    }
    else
        return true ;
}

// Функция удаляет из таблицы Курсы по id курса
bool DataModule::deleteClassById(const QString &id)
{
    QSqlQuery query ;
    // запрос на удаление из таблицы Курсы значения по id
    query.prepare(QString("DELETE FROM classes WHERE id = " + id));
    if (!query.exec()) {
        ErrMsg = "Ошибка удаления курса" ;
        return false ;
    }
    else
        return true ;
}

// Функция удаляет из таблицы Обучающиеся по id обучающегося
bool DataModule::deleteStudentById(const QString &id)
{
    QSqlQuery query ;
    // запрос на удаление из таблицы Обучающиеся значения по id
    query.prepare(QString("DELETE FROM students WHERE id = " + id));
    if (!query.exec()) {
        ErrMsg = "Ошибка удаления студента" ;
        return false ;
    }
    else
        return true ;
}

// Функция удаляет из таблицы Языки по id языка
bool DataModule::deleteLangById(const QString &id)
{
    QSqlQuery query ;
    // запрос на удаление из таблицы Языки значения по id
    query.prepare(QString("DELETE FROM languages WHERE id = " + id));
    if (!query.exec()) {
        ErrMsg = "Ошибка удаления языка обучения" ;
        return false ;
    }
    else
        return true ;
}

// Функция удаляет из таблицы Уровни Курсов по id уровня
bool DataModule::deleteLevelById(const QString &id)
{
    QSqlQuery query ;
    // запрос на удаление из таблицы Уровни Курсов значения по id
    query.prepare(QString("DELETE FROM levels WHERE id = " + id));
    if (!query.exec()) {
        ErrMsg = "Ошибка удаления уровня курсов" ;
        return false ;
    }
    else
        return true ;
}

// Функция удаляет из таблицы Уровни Преподавания по id уровня
bool DataModule::deleteTLevelById(const QString &id)
{
    QSqlQuery query ;
    // запрос на удаление из таблицы Уровни Преподавателей значения по id
    query.prepare(QString("DELETE FROM teacherlevels WHERE id = " + id));
    if (!query.exec()) {
        ErrMsg = "Ошибка удаления уровня преподавателей" ;
        return false ;
    }
    else
        return true ;
}

// Функция удаляет из таблицы Запись на Курсы по id записи
bool DataModule::deleteReservesById(const QString &id)
{
    QSqlQuery query ;
    // запрос на удаление из таблицы Запись На Курсы значения по id
    query.prepare(QString("DELETE FROM reserves WHERE id = " + id));
    if (!query.exec()) {
        ErrMsg = "Ошибка удаления записи на курсы" ;
        return false ;
    }
    else
        return true ;
}
