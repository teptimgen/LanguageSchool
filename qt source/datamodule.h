#ifndef DATAMODULE_H
#define DATAMODULE_H

#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlQuery>
#include <QtSql/QSqlError>
#include <QMap>
#include <QVariant>

// Класс предназначен для соединения с базой данных,
// а также для выполнения действий над ней
class DataModule
{
private:
    QSqlDatabase sdb ;
    QString ErrMsg ;
public:
    DataModule();
    bool Connect() ;
    QString getErrMsg() const ;
    QMap<int,QString> getLanguages() ;
    QMap<int,QString> getLevels() ;
    QMap<int,QString> getTeacherLevels() ;
    bool addLanguage(const QString & language);
    bool addLevel(const QString & level) ;
    bool addTeacherLevel(const QString & level) ;

    bool deleteClassById(const QString &id);
    bool deleteStudentById(const QString &id);
    bool deleteLangById(const QString &id);
    bool deleteLevelById(const QString &id);
    bool deleteTLevelById(const QString &id);
    bool deleteReservesById(const QString &id);
};

extern DataModule DM ;

#endif // DATAMODULE_H
