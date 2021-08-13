#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include<QMouseEvent>
#include <QKeyEvent>
#include <QString>
#include <QFont>
#include <QRect>
#include <QPainter>
#include <QTimer>
#include <QAction>
#include<QDebug>
#include<QFileDialog>
#include <QFile>
#include<tuple>
#include<sstream>
#include"snake.h"
using std::tuple;
using std::stringstream;

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

private:
    Ui::MainWindow *ui;
    int dot0,dot1;
    const int Itv_= 10;
    bool Pause_;
    pii ap;
    int Score;
    int Time_;

public:
    snake ss;
    int Rand(int l,int r);
    QRect getqrect(int x,int y, int d=0);
    tuple<int,int,int,int> getRect(int x,int y);
    void ChangeInfo(QString qs);
    void ChangeScore(int x);
    void ChangeTime(int x);
    pii Getpos(QPoint qp);
    void clear();
    void Print();
    void setButton_Ready();
    void setButton_Started();
    void setButton_Pause();
    void setButton_GameOver();

    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void paintEvent(QPaintEvent *ev);
    void keyPressEvent(QKeyEvent*);
    void mousePressEvent(QMouseEvent *ev);
    void mouseReleaseEvent(QMouseEvent *ev);
    void mouseMoveEvent(QMouseEvent *ev);

    void PaintSnake();
    void Gen_apple();
    void GameOver();


public slots:
    void Start();
    void Restart();
    void Pause();
    void Continue();
    void Load();
    void Save();
    void UPD();
private slots:
    void on_Startaction_triggered();
    void on_Restartaction_triggered();
    void on_Quitaction_triggered();
    void on_Continueaction_triggered();
    void on_Pauseaction_triggered();
    void on_Saveaction_triggered();
    void on_Loadaction_triggered();
};

#endif // MAINWINDOW_H
