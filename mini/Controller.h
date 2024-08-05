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

using namespace std;

class Controller;
class Cell;
class GridWidget;
class ResizeWidget;

extern std::shared_ptr<GridWidget> widget;

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
    ClearGrid
};


struct Point {
    int x, y;
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

    Controller(QWidget* parent = nullptr) : QWidget(parent)
    {
        setupController();
    }

    vector<Point> BFS();
//private:
    void setupController();
    typeButton _status;
    QButtonGroup* _buttonGroup = new QButtonGroup(this);
    Cell* _source = nullptr;
    Cell* _target = nullptr;
    vector<Point> _path;
    int _clearMode = 0;
    ResizeWidget _resize;
    bool _isClearGrid = false;
};

