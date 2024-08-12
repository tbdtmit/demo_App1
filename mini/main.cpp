
#include <iostream>
#include <qmainwindow.h>
#include <qpushbutton.h>
#include <qboxlayout.h>
#include <qmessagebox.h>
#include <qapplication.h>


#include "Controller.h"
#include "Map.h"

std::shared_ptr<Game> Game::demo;
std::shared_ptr<Controller> Game::controller;
std::shared_ptr<GridWidget> Game::widget;
//Game* demo;
int main(int argc, char* argv[])
{
    QApplication app(argc, argv);
    Game::demo = std::make_shared<Game>();
    Game::controller = std::make_shared<Controller>(&*Game::demo);
    Game::widget = std::make_shared<GridWidget>(32, 32, &*Game::demo);
    Game::demo->launch();
    Game::demo->show();
    return app.exec();
}


