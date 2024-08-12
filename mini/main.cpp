
#include <iostream>
#include <qmainwindow.h>
#include <qpushbutton.h>
#include <qboxlayout.h>
#include <qmessagebox.h>
#include <qapplication.h>


#include "Controller.h"
#include "Map.h"


std::shared_ptr<Controller> Game::controller;
std::shared_ptr<GridWidget> Game::widget;

int main(int argc, char* argv[])
{
    QApplication app(argc, argv);
    Game::controller = std::make_shared<Controller>();
    Game::widget = std::make_shared<GridWidget>();
    Game demo;
    demo.launch();


    return app.exec();
}


