
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
    widget = std::make_shared<GridWidget>();
    controller = std::make_shared<Controller>();
    QWidget w;
    QMessageBox::information(&w, "Message","Day la ban demo nhanh chua duoc toi uu hieu nang va con nhieu thieu xot. Neu anh/chi co yeu cau nao khac, em rat mong nhan duoc gop y de san pham duoc hoan thien. Xin cam on.");
    widget->show();
    controller->show();

    return app.exec();
}


