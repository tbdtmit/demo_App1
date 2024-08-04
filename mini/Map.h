#pragma once

#include <qwidget.h>
#include <qgridlayout.h>
#include <qsignalmapper.h>
#include <qpushbutton.h>
#include "Controller.h"
#include <qevent.h>
#include <qpainter.h>

class GridWidget;
class Controller;

extern std::shared_ptr<Controller> controller;


class Cell : public QRect
{
public:
    enum typeCell
    {
        UnBlocked,
        Blocked,
        Target,
        Source,
        Path
    };

    void setLocation(int x, int y);
    void setSource();
    void setTarget();
    void setBlocked();
    void setUnBlocked();

    typeCell _type = typeCell::UnBlocked;
    int _x;
    int _y;
    Point _parentOnBFS;
};

class GridWidget : public QWidget
{

public:
    GridWidget(int maxX = 32, int maxY = 32, QWidget* parent = nullptr);

    void mousePressEvent(QMouseEvent* event);
    void paintEvent(QPaintEvent* event) override;
    void gridUpdate(QMouseEvent* event);
//private:
    void setupGrid();
    int _maxX;
    int _maxY;
    void drawPath();

    int _cellSize = 15;
    int _gridSizeRow;
    int _gridSizeCol;
    QVector<QVector<std::shared_ptr<Cell>>> _gridRects;
    QPoint _clickedCell;

};

