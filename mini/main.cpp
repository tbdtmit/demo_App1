
#include <iostream>
#include <qmainwindow.h>
#include <qpushbutton.h>
#include <qboxlayout.h>
#include <qmessagebox.h>
#include <qapplication.h>


#include "Controller.h"
#include "Map.h"


std::shared_ptr<Controller> controller;
std::shared_ptr<GridWidget> widget;

int main(int argc, char* argv[])
{
    QApplication app(argc, argv);
    controller = std::make_shared<Controller>();
    widget = std::make_shared<GridWidget>();

    widget->show();
    controller->move(widget->x() - controller->width() - 20, widget->y());  
    controller->show();

    return app.exec();
}


