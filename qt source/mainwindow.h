#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

// Класс Главной формы приложения
class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    // Обработчики (слоты) нажатия на кнопки, расположенные на форме
private slots:
    void on_butAddClass_clicked();

    void on_butEditClass_clicked();

    void on_butDelClass_clicked();

    void on_butAddToClass_clicked();
//--------------------------------------------

    void on_butAddStud_clicked();

    void on_butEditStud_clicked();

    void on_butDelStud_clicked();
//--------------------------------------------

    void on_butAddLang_clicked();

    void on_butEditLang_clicked();

    void on_butDelLang_clicked();
//--------------------------------------------

    void on_butAddLevel_clicked();

    void on_butEditLevel_clicked();

    void on_butDelLevel_clicked();
//--------------------------------------------

    void on_butAddTLevel_clicked();

    void on_butEditTLevel_clicked();

    void on_butDelTLevel_clicked();
//--------------------------------------------

   void on_butEditRes_clicked();

   void on_butDelRes_clicked();

private:
    Ui::MainWindow *ui;
    // Функции обновления таблиц, отображаемых на форме
    void reserves2table(int id=0) ;
    void classes2table(int id=0) ;
    void students2table(int id=0) ;
    void languages2table(int id=0) ;
    void levels2table(int id=0) ;
    void teacherlevels2table(int id=0) ;
};
#endif // MAINWINDOW_H
