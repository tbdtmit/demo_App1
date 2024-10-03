#pragma once

#include <qwidget.h>
#include <qgridlayout.h>
#include <qpushbutton.h>
#include <iostream>
#include <qbuttongroup.h>
#include <queue>
#include <qlineedit.h>
#include <qlabel.h>
#include <qintvalidator>
#include <qmessagebox.h>
#include <qcoreapplication.h>
#include <qtimer.h>
#include <qthread.h>

using namespace std;

class Controller;
class Cell;
class GridWidget;
class ResizeWidget;

//::std::shared_ptr<GridWidget> Game::widget;


enum typeButton
{
    UnBlocked,
    Blocked,
    Target,
    Source,
    Starting,
    ClearPath,
    Resize,
    Reset,
    ClearGrid,
    StopFinding,
    SetTimeOnFrame
};


struct Point {
    int x, y;
};

struct DetailPoint {
    double h, g, f = FLT_MAX;
};

struct CompareFirst {
    bool operator()(const pair<double, Point>& a, const pair<double, Point>& b) const {
        return a.first > b.first; 
    }
};

class ControllerBut : public QPushButton
{
public:

    ControllerBut(typeButton type, Controller* parent);
    void handleClick();

    typeButton _type;
    Controller* _controller;

};

class ResizeWidget : public QWidget {
public:
    ResizeWidget(QWidget* parent = nullptr);

private slots:
    void onResizeButtonClicked();

private:
    QLineEdit* widthEdit;
    QLineEdit* heightEdit;
    QPushButton* resizeButton;
};

class SetTimeWidget : public QWidget {
public:
    SetTimeWidget(QWidget* parent = nullptr);

private slots:
    void onSetTimeButtonClicked();

private:
    QLineEdit* setTimeEdit;
    QPushButton* setTimeButton;
};

class Controller : public QWidget
{
    
public:
	enum Status
	{
        UnBlocked,
        Blocked,
        Target,
        Source,
        Finding,
        ClearPath
	};

    Controller(QWidget* parent = nullptr);
    vector<Point> Astar();
//private:
    void setupController();
    typeButton _status;
    QButtonGroup* _buttonGroup = new QButtonGroup(this);
    std::weak_ptr<Cell> _source;
    std::weak_ptr<Cell> _target;
    vector<Point> _path;
    ResizeWidget _resize;
    SetTimeWidget _setTimeOnFrame;
    bool _isClearGrid = false;
    bool _isOnBFS = false;
    double _timeOnFrame = 1;



};
