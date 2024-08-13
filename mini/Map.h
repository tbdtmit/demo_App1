#pragma once

#include <qwidget.h>
#include <qgridlayout.h>
#include <qsignalmapper.h>
#include <qpushbutton.h>
#include "Controller.h"
#include <qevent.h>
#include <qpainter.h>
#include <qapplication.h>
#include <qscreen.h>

class GridWidget;
class Controller;

//::std::shared_ptr<Controller> Game::controller;


class Cell : public QRect
{
public:
    enum typeCell
    {
        UnBlocked,
        Blocked,
        Target,
        Source,
        Path,
        StepOnBFS
    };

    void setLocation(int x, int y);
    void setSource();
    void setTarget();
    void setBlocked();
    void setUnBlocked();
    void setStepOnBFS();

    Cell::typeCell _type = typeCell::UnBlocked;
    int _x;
    int _y;
    Point _parentOnBFS;
};

class GridWidget : public QWidget
{

public:
    GridWidget(int maxX = 32, int maxY = 32, QWidget* parent = nullptr);

    void mousePressEvent(QMouseEvent* event);
    void mouseMoveEvent(QMouseEvent* event) override;
    void mouseReleaseEvent(QMouseEvent* event) override;
    void paintEvent(QPaintEvent* event) override;
    void resizeEvent(QResizeEvent* event) override;
    //void enterEvent(QEvent* event) override;
    void mapUpdate(QMouseEvent* event);
    void drawInitialMap(QPainter& paintermap, QPainter& paintergrid, int i, int j);
    void drawMapAfterResize(QPainter& paintermap, QPainter& paintergrid, int i, int j);
//private:
    void setupInitialMap();
    void drawPath();
    void clearPath();


    int _maxRow;
    int _maxCol;

    float _cellSizeX = 15;
    float _cellSizeY = 15;
    float _gridSizeRow;
    float _gridSizeCol;
    QVector<QVector<std::shared_ptr<Cell>>> _gridRects;
    QPoint _clickedCell;
    bool _isDragging = false;
    QPixmap _map;
    QPixmap _grid;





};

class Game : public QWidget{

public:
    static std::shared_ptr<Game> demo;
    Game(QWidget* parent = nullptr);

    void launch()
    {

        //Game::controller->move(Game::widget->x() - Game::controller->width() - 20, Game::widget->y());
        //QHBoxLayout* layout = new QHBoxLayout(&*demo);
        layout1->addWidget(&*Game::widget);
        layout2->addWidget(&*Game::controller);
        mainLayout->addLayout(layout1);
        mainLayout->addLayout(layout2);
        demo->setLayout(mainLayout);


        //Game::widget->show();

        //Game::controller->show();
    }
     QHBoxLayout* layout1;
     QHBoxLayout* layout2;
     QHBoxLayout* mainLayout;
    static std::shared_ptr<Controller> controller;
    static std::shared_ptr<GridWidget> widget;
};