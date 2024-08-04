#include "Controller.h"
#include "Map.h"


bool isValid(int x, int y, int rows, int cols) {
    return x >= 1 && x <= rows && y >= 1 && y <= cols;
}


vector<Point> Controller::BFS()
{
    Point start = { _source->_x, _source->_y };
    Point end = { _target->_x, _target->_y };

    vector<vector<bool>> visited(widget->_maxX + 1, vector<bool>(widget->_maxY + 1, false));
    queue<Point> q;

    q.push(start);
    visited[start.x][start.y] = true;

    const vector<Point> directions = { {0, 1}, {1, 0}, {0, -1}, {-1, 0}, {-1, 1}, {1, 1}, {1, -1}, {-1, -1} }; // Các hướng di chuyển: 8 hướng

    while (!q.empty()) {
        Point current = q.front();
        q.pop();

        for (const Point& dir : directions) {
            Point next = { current.x + dir.x, current.y + dir.y };
            if (isValid(next.x, next.y, widget->_maxX, widget->_maxY) 
                && visited[next.x][next.y] == false
                && widget->_gridRects[next.x][next.y]->_type != Cell::typeCell::Blocked )
            {
                visited[next.x][next.y] = true;
                q.push(next);
                widget->_gridRects[next.x][next.y]->_parentOnBFS = current;
                if (next.x == end.x && next.y == end.y)
                {
                    vector<Point> path;
                    while (!(next.x == start.x && next.y == start.y))
                    {
                        path.push_back(widget->_gridRects[next.x][next.y]->_parentOnBFS);
                        next = widget->_gridRects[next.x][next.y]->_parentOnBFS;
                    }
                    return path;
                }
            }
        }
    }

    return {}; // Không tìm thấy đường đi
}

void Controller::setupController()
{
    QGridLayout* layout = new QGridLayout(this);
    typeButton buttons[8] = {
        typeButton::UnBlocked,
        typeButton::Blocked,
        typeButton::Target,
        typeButton::Source,
        typeButton::Starting,
        typeButton::ClearPath,
        typeButton::Resize,
        typeButton::Reset };

    for (int row = 0; row < 8; ++row) {
        QPushButton* button = new ControllerBut(buttons[row], this);
        button->setCheckable(true);
        _buttonGroup->addButton(button);
        layout->addWidget(button, row, 1);

    }

    setLayout(layout);

}


ControllerBut::ControllerBut(typeButton type, Controller* parent)
    : _type(type),
    _controller(parent)
{
    this->setFixedSize(64, 32);
    switch (type)
    {
    case typeButton::UnBlocked:
        this->setText("UnBlocked");
        this->setStyleSheet("background-color: white"); // Đặt màu nền cho nút
        break;
    case typeButton::Blocked:
        this->setText("Blocked");
        this->setStyleSheet("background-color: gray"); // Đặt màu nền cho nút
        break;
    case typeButton::Target:
        this->setText("Target");
        this->setStyleSheet("background-color: red"); // Đặt màu nền cho nút
        break;
    case typeButton::Source:
        this->setText("Source");
        this->setStyleSheet("background-color: green"); // Đặt màu nền cho nút
        break;
    case typeButton::Starting:
        this->setText("Starting");
        this->setStyleSheet("background-color: white"); // Đặt màu nền cho nút
        break;
    case typeButton::ClearPath:
        this->setText("Clear Path");
        this->setStyleSheet("background-color: white"); // Đặt màu nền cho nút
        break;
    case typeButton::Reset:
        this->setText("Reset");
        this->setStyleSheet("background-color: white"); // Đặt màu nền cho nút
        break;
    case typeButton::Resize:
        this->setText("Resize");
        this->setStyleSheet("background-color: white"); // Đặt màu nền cho nút
        break;
    default:
        break;
    }

    QObject::connect(this, &QPushButton::clicked, [&]() {
        handleClick();
        });
}


void ControllerBut::handleClick()
{

    std::cout << this->text().toStdString() << std::endl;
    _controller->_status = this->_type;
    if (_controller->_status == typeButton::Starting)
    {
        if (_controller->_source && _controller->_target && _controller->_source->isValid() && _controller->_target->isValid())
        {
            for (auto point : controller->_path)
            {
                    //widget->gridRects[point.x][point.y]->_index = 0;
                    if (widget->_gridRects[point.x][point.y]->_type == Cell::typeCell::Path)
                        widget->_gridRects[point.x][point.y]->_type = Cell::typeCell::UnBlocked;
                }
            controller->_path.clear();
            controller->_path = _controller->BFS();
            widget->drawPath();

        }
        else
        {
            QMessageBox::information(this, "Message", "Starting: chon source va target de bat dau tim kiem.");
        }
      

    }
    else if (_controller->_status == typeButton::ClearPath)
    {

        for (auto point : controller->_path)
        {
                if(widget->_gridRects[point.x][point.y]->_type == Cell::typeCell::Path)
                    widget->_gridRects[point.x][point.y]->_type = Cell::typeCell::UnBlocked;          
        }
        controller->_path.clear();

    }
    else if (_controller->_status == typeButton::Reset)
    {
        //widget.reset();
        controller->_source = nullptr;
        controller->_target = nullptr;
        controller->_path.clear();
        widget = std::make_shared<GridWidget>();
        widget->show();
        return;
    }
    else if (_controller->_status == typeButton::Resize)
    {
        if (controller->_resize.isVisible())
        {
            controller->_resize.close();
        }
        controller->_resize.resize(200, 150);
        controller->_resize.show();
    }
    widget->update();
}


ResizeWidget::ResizeWidget(QWidget* parent)
    : QWidget(parent) {
    // Create labels and input fields
    QLabel* widthLabel = new QLabel("Width (32-256):", this);
    QLabel* heightLabel = new QLabel("Height (32-256):", this);
    widthEdit = new QLineEdit(this);
    heightEdit = new QLineEdit(this);

    // Create the resize button
    resizeButton = new QPushButton("Resize", this);


    QIntValidator* intValidator = new QIntValidator(32, 256, this); // Adjust range as needed
    widthEdit->setValidator(intValidator);
    heightEdit->setValidator(intValidator);


    // Create layout for the input fields
    QHBoxLayout* widthLayout = new QHBoxLayout;
    widthLayout->addWidget(widthLabel);
    widthLayout->addWidget(widthEdit);

    QHBoxLayout* heightLayout = new QHBoxLayout;
    heightLayout->addWidget(heightLabel);
    heightLayout->addWidget(heightEdit);

    // Create main layout
    QVBoxLayout* mainLayout = new QVBoxLayout(this);
    mainLayout->addLayout(widthLayout);
    mainLayout->addLayout(heightLayout);
    mainLayout->addWidget(resizeButton);

    // Set the main layout for the widget
    setLayout(mainLayout);

    // Connect the resize button click signal to the slot
    connect(resizeButton, &QPushButton::clicked, this, &ResizeWidget::onResizeButtonClicked);
}

void ResizeWidget::onResizeButtonClicked() {
    // Get the input values
    int width = widthEdit->text().toInt();
    int height = heightEdit->text().toInt();

    // Resize the widget
    //widget.reset();
    if (width >= 32 && width <= 256 && height >= 32 && height <= 256)
    {
        controller->_source = nullptr;
        controller->_target = nullptr;
        controller->_path.clear();
        widget = std::make_shared<GridWidget>(width, height);
        widget->show();
    }
    this->close();
}