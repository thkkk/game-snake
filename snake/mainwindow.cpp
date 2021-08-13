#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    dot0 = 85;
    dot1 = 885;
    Pause_ = 1;
    Score=0;
    Time_ = 0;
    ui->setupUi(this);

    connect(ui->StartButton, SIGNAL(clicked()),this,SLOT(Start()));
    connect(ui->RestartButton, SIGNAL(clicked()) , this, SLOT(Restart()));
    connect(ui->PauseButton, SIGNAL(clicked()) , this, SLOT(Pause()));
    connect(ui->ContinueButton,SIGNAL(clicked()) ,this , SLOT(Continue()));
    connect(ui->SaveButton , SIGNAL(clicked()) , this , SLOT(Save()));
    connect(ui->LoadButton, SIGNAL(clicked()) ,this , SLOT(Load()));
    connect(ui->QuitButton,SIGNAL(clicked()),this, SLOT(close()));

    //connect(ui->menuBar,SIGNAL(triggered(QAction*)),this,SLOT(trigerMenu(QAction*))); //??
    setButton_Ready();

    QTimer *timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(UPD()));
    timer->start(100);
}

MainWindow::~MainWindow()
{
    delete ui;
}
int MainWindow::Rand(int l, int r)
{
    return rand()%(r-l+1)+1;
}

tuple<int,int,int,int> MainWindow::getRect(int x,int y)
{
    int d = (dot1-dot0)/40;
    int sx = dot0 + (x-1)*d , sy=dot0+ (y-1)*d;
    return std::make_tuple(sx,sy,d,d);
}


QRect MainWindow::getqrect(int x,int y,int d)
{
    tuple<int,int,int,int> t = getRect(x,y);
    return QRect(std::get<0>(t)-d,std::get<1>(t)-d,std::get<2>(t)+2*d,std::get<3>(t)+2*d);
}

void MainWindow::ChangeInfo(QString qs)
{
    ui->Infolabel->setText(qs);
}
void MainWindow::ChangeScore(int x)
{
    Score += x;
    QString qs = "Score: ";
    qs += QString::number(Score);
    ui->Scorelabel->setText(qs);
}
void MainWindow::ChangeTime(int x)
{
    Time_ += x;
    QString qs = "Time: ";
    qs += QString::number(Time_);
    ui->Timelabel->setText(qs);
}

pii MainWindow::Getpos(QPoint qp)
{
    int d = (dot1-dot0)/40;
    int x = (qp.x()-dot0)/d + 1;
    int y = (qp.y()-dot0)/d + 1;
    return pii(x,y);
}
void MainWindow::clear()
{
    ChangeScore(-Score);
    ChangeTime(-Time_);
    Time_ =0 ;
    Score=0;
    ss.clear();
}
void MainWindow::setButton_Ready()
{
    ui->Startaction->setEnabled(1);
    ui->Restartaction->setEnabled(0);
    ui->Loadaction->setEnabled(1);
    ui->Continueaction->setEnabled(0);
    ui->Pauseaction->setEnabled(0);
    ui->Saveaction->setEnabled(0);
    ui->Quitaction->setEnabled(1);

    ui->StartButton->setEnabled(1);
    ui->RestartButton->setEnabled(0);
    ui->LoadButton->setEnabled(1);
    ui->ContinueButton->setEnabled(0);
    ui->PauseButton->setEnabled(0);
    ui->SaveButton->setEnabled(0);
    ui->QuitButton->setEnabled(1);
}
void MainWindow::setButton_Started()
{
    ui->Startaction->setEnabled(0);
    ui->Restartaction->setEnabled(0);
    ui->Loadaction->setEnabled(0);
    ui->Continueaction->setEnabled(0);
    ui->Pauseaction->setEnabled(1);
    ui->Saveaction->setEnabled(0);
    ui->Quitaction->setEnabled(1);

    ui->StartButton->setEnabled(0);
    ui->RestartButton->setEnabled(0);
    ui->LoadButton->setEnabled(0);
    ui->ContinueButton->setEnabled(0);
    ui->PauseButton->setEnabled(1);
    ui->SaveButton->setEnabled(0);
    ui->QuitButton->setEnabled(1);
}
void MainWindow::setButton_Pause()
{
    ui->Startaction->setEnabled(0);
    ui->Restartaction->setEnabled(1);
    ui->Loadaction->setEnabled(0);
    ui->Continueaction->setEnabled(1);
    ui->Pauseaction->setEnabled(0);
    ui->Saveaction->setEnabled(1);
    ui->Quitaction->setEnabled(1);

    ui->StartButton->setEnabled(0);
    ui->RestartButton->setEnabled(1);
    ui->LoadButton->setEnabled(0);
    ui->ContinueButton->setEnabled(1);
    ui->PauseButton->setEnabled(0);
    ui->SaveButton->setEnabled(1);
    ui->QuitButton->setEnabled(1);
}
void MainWindow::setButton_GameOver()
{
    ui->Startaction->setEnabled(0);
    ui->Restartaction->setEnabled(1);
    ui->Loadaction->setEnabled(0);
    ui->Continueaction->setEnabled(0);
    ui->Pauseaction->setEnabled(0);
    ui->Saveaction->setEnabled(0);
    ui->Quitaction->setEnabled(1);

    ui->StartButton->setEnabled(0);
    ui->RestartButton->setEnabled(1);
    ui->LoadButton->setEnabled(0);
    ui->ContinueButton->setEnabled(0);
    ui->PauseButton->setEnabled(0);
    ui->SaveButton->setEnabled(0);
    ui->QuitButton->setEnabled(1);
}

//////////////////////

void MainWindow::Start()
{
    if(ui->Infolabel->text() == "未开始游戏")
    {
        Pause_=0;
        setButton_Started();
        ChangeInfo("已开始游戏");
        Gen_apple();
    }
}
void MainWindow::Restart()
{
    if(ui->Infolabel->text() == "游戏结束！" || (ui->Infolabel->text() == "已开始游戏" && Pause_ == 1))
    {
        clear();
        Pause_=1;
        setButton_Ready();
        ChangeInfo("未开始游戏");
        update();
    }
}
void MainWindow::Pause()
{
    if(ui->Infolabel->text() == "已开始游戏")
    {
        setButton_Pause();
        Pause_ = 1;
    }
}
void MainWindow::Continue()
{
    if(ui->Infolabel->text() == "已开始游戏")
    {
        setButton_Started();
        Pause_ = 0;
    }
}
void MainWindow::Load()
{
    if(ui->Infolabel->text() != "未开始游戏") return ;
    QString filename = QFileDialog::getOpenFileName(this , "载入游戏" , "./data/" , "*.snake");
    QFile file(filename);

    if(file.open(QIODevice::ReadWrite |QIODevice::Text))
    {

        QTextStream in(&file);
        QString str = in.readAll();
        stringstream s;
        s << str.toStdString();

        s >> dot0 >>dot1;
        s>> Pause_;
        s >> ap.first >>ap.second;
        s>>Score;
        s>> Time_;
        s >> ss.last_move_direc;
        s>> ss.rest ;
        int sz; s >> sz;
        ss.body.clear();
        for(int i=0;i<sz;++i)
        {
            pii x;
            s>> x.first >>x.second;
            ss.body.push_back(x);
        }
        s>>  ss.direc;
        for(int i=0;i<=41; ++i) for(int j=0;j<=41;++j)
            s>> ss.Map[i][j];
        file.close();
        setButton_Pause();
        ChangeInfo("已开始游戏");
        ChangeScore(0);
        ChangeTime(0);
        update();
    }
}
void MainWindow::Save()
{
    if(ui->Infolabel->text() != "已开始游戏" || Pause_ != 1) return ;
    QString filename = QFileDialog::getSaveFileName(this , "保存游戏" , "./data/" , "*.snake");
    QFile file(filename);
    if(file.open(QIODevice::WriteOnly|QIODevice::Text))
    {
        QTextStream out(&file);
        out << dot0 <<' ' <<dot1<<' ';

        out<< Pause_<<' ';
        out << ap.first <<' '<<ap.second<<' ';
        out<<Score<<' ';
        out<< Time_<<' ';
        out << ss.last_move_direc<<' ';
        out<< ss.rest <<' ';
        out << int(ss.body.size())<<' ' ;
        for(pii x : ss.body)
            out<< x.first <<' '<<x.second<<' ';
        out<<endl<<  ss.direc<<' ';
        for(int i=0;i<=41; ++i) for(int j=0;j<=41;++j)
            out<< ss.Map[i][j] <<' ';
        out.flush();
        file.close();
    }
}


void MainWindow::Gen_apple()
{
    while(1)
    {
        int x=Rand(1,40),y=Rand(1,40);
        if(ss.Map[x][y] == 0)
        {
            ss.Map[x][y]=3;
            ap = pii(x,y);
            break;
        }
    }//What if there is no pos to gen?
}

void MainWindow::paintEvent(QPaintEvent *ev)
{
    QPainter p(this);
    QLinearGradient g(dot0-Itv_,dot0-Itv_,dot1+Itv_,dot1+Itv_);
    g.setColorAt(0.0, Qt::red);
    g.setColorAt(1.0, Qt::yellow);
    p.setPen(Qt::black);
    p.setBrush(g);
    p.drawRect(dot0-Itv_,dot0-Itv_,dot1-dot0+2*Itv_,dot1-dot0+2*Itv_);

    p.setPen(Qt::black);
    p.setBrush(Qt::black);
    p.drawRect(dot0,dot0,dot1-dot0,dot1-dot0);

    p.setPen(Qt::gray);
    for(int d=(dot1-dot0)/40,i=1;i<=39;++i)
    {
        p.drawLine(dot0+i*d,dot0, dot0+i*d, dot1);
        p.drawLine(dot0,dot0+i*d, dot1, dot0+i*d);
    }
    //Draw wall
    for(int i=1;i<=40;++i) for(int j=1;j<=40;++j)
    {
        if(ss.Map[i][j]== 2)
            p.drawPixmap(getqrect(i,j), QPixmap("wall.jpg"));
    }

    //Draw snake
    for(int sz=ss.body.size(),i=sz-1;i>=0;--i)
    {
        if(i==0)
        {
            if(ui->Infolabel->text() == "游戏结束！")
                p.drawPixmap(getqrect(ss.body[0].first, ss.body[0].second,3), QPixmap("cry.png"));
            else
                p.drawPixmap(getqrect(ss.body[0].first, ss.body[0].second,3), QPixmap("head.png"));
        }
        else
        {
            /*p.setPen(Qt::gray);
            p.setBrush(Qt::green);
            p.drawRect(getqrect(ss.body[i].first, ss.body[i].second));*/
            p.drawPixmap(getqrect(ss.body[i].first, ss.body[i].second), QPixmap("snake.jpg"));
        }
    }

    //Draw apple
    if(ui->Infolabel->text() != "未开始游戏")
        p.drawPixmap(getqrect(ap.first, ap.second), QPixmap("apple.png"));

}


void MainWindow::UPD()
{
    if(!Pause_)
    {
        ChangeTime(1);
        int res=ss.move();
        if(res == 1)
        {
            ChangeScore(3);
            Gen_apple();
        }
        update();
        if(res == 2) GameOver();
    }
}

void MainWindow::GameOver()
{
    Pause_ = 1;
    setButton_GameOver();
    ChangeInfo("游戏结束！");
}

void MainWindow::keyPressEvent(QKeyEvent* ev)
{
    switch(ev->key())
    {
        case Qt::Key_Up : ss.ChangeDirec(0);
        break;
        case Qt::Key_Down: ss.ChangeDirec(2);
        break;
        case Qt::Key_Left: ss.ChangeDirec(3);
        break;
        case Qt::Key_Right: ss.ChangeDirec(1);
        break;
    }
}

void MainWindow::mousePressEvent(QMouseEvent *ev)
{
    if(ui->Infolabel->text() != "未开始游戏") return ;
    pii pos = Getpos(ev->pos());
    if(pos.first<1 || pos.first >40 || pos.second<1 || pos.second>40) return ;
    if(ss.Map[pos.first][pos.second] != 1)
    {
        (ss.Map[pos.first][pos.second] += 2)%=4;
    }
    update();
}
void MainWindow::mouseReleaseEvent(QMouseEvent *ev)
{
    if(ui->Infolabel->text() != "未开始游戏") return ;
    pii pos = Getpos(ev->pos());
    if(pos.first<1 || pos.first >40 || pos.second<1 || pos.second>40) return ;
    if(ss.Map[pos.first][pos.second] != 1)
    {
        //(ss.Map[pos.first][pos.second] += 2)%=4;
    }
    update();
}
void MainWindow::mouseMoveEvent(QMouseEvent *ev)
{
    if(ui->Infolabel->text() != "未开始游戏") return ;
    pii pos = Getpos(ev->pos());
    if(pos.first<1 || pos.first >40 || pos.second<1 || pos.second>40) return ;
    if(ss.Map[pos.first][pos.second] != 1)
    {
        (ss.Map[pos.first][pos.second] += 2)%=4;
    }
    update();
}


void MainWindow::on_Startaction_triggered()
{
    Start();
}

void MainWindow::on_Restartaction_triggered()
{
    Restart();
}

void MainWindow::on_Quitaction_triggered()
{
    close();
}

void MainWindow::on_Continueaction_triggered()
{
    Continue();
}

void MainWindow::on_Pauseaction_triggered()
{
    Pause();
}

void MainWindow::on_Saveaction_triggered()
{
    Save();
}

void MainWindow::on_Loadaction_triggered()
{
    Load();
}
