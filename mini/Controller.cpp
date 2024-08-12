#include "Controller.h"
#include "Map.h"


bool isValid(int x, int y, int rows, int cols) {
    return x >= 1 && x <= rows && y >= 1 && y <= cols;
}

void delay(float milliseconds) {
    QEventLoop loop;
    QTimer::singleShot(milliseconds, &loop, &QEventLoop::quit);
    loop.exec();
}

void clearStepsOnBFS(vector<Point> stepsOnBFS)
{
    if (stepsOnBFS.size())
    {
        for (auto cell : stepsOnBFS)
        {
            if(Game::widget->_gridRects[cell.x][cell.y]->_type != Cell::typeCell::Blocked)
                Game::widget->_gridRects[cell.x][cell.y]->setUnBlocked();
        }
    }

}

double calculateHeuristicValue(int row, int col, Point target) {
    //return abs(row - target.x) > abs(col - target.y) ? abs(row - target.x) : abs(col - target.y);
    return sqrt(pow(row - target.x, 2) + pow(col - target.y, 2));
}

Controller::Controller(QWidget* parent) : QWidget(parent)
{
    this->setFixedWidth(100);
    setupController();

}

vector<Point> Controller::BFS()
{
    _isOnBFS = true;
    Point start = { _source->_x, _source->_y };
    Point end = { _target->_x, _target->_y };

    vector<vector<bool>> visited(Game::widget->_maxRow + 1, vector<bool>(Game::widget->_maxCol + 1, false));
    vector<vector<DetailPoint>> details(Game::widget->_maxRow + 1, vector<DetailPoint>(Game::widget->_maxCol + 1));
    //queue<Point> q;
    priority_queue<pair<double, Point>, vector<pair<double, Point>>, CompareFirst> q;

    q.push({ 0.0,start });
    visited[start.x][start.y] = true;

    const vector<Point> directions = { {0, 1}, {1, 0}, {0, -1}, {-1, 0}, {-1, 1}, {1, 1}, {1, -1}, {-1, -1} }; // Các hướng di chuyển: 8 hướng
    vector<Point> _stepsOnBFS;
    while (!q.empty()) {
        auto current = q.top();
        q.pop();
        visited[current.second.x][current.second.y] = true;
        double gNew, hNew, fNew;
        int cnt = 0;
        for (const Point& dir : directions) {
            ++cnt;
            Point next = { current.second.x + dir.x, current.second.y + dir.y };
            if (isValid(next.x, next.y, Game::widget->_maxRow, Game::widget->_maxCol)
                && visited[next.x][next.y] == false
                && Game::widget->_gridRects[next.x][next.y]->_type != Cell::typeCell::Blocked
                && Game::widget->_gridRects[current.second.x][current.second.y]->_type != Cell::typeCell::Blocked)
            {

                gNew = details[current.second.x][current.second.y].g + 1.0; // trong TH chi phi di chuyen sang moi o la nhu nhau(cheo, doc, ngang) \\\ su dung + ((cnt < 4) ? 1.0 : sqrt(2.0)); neu duong cheo duoc tinh la hon doc va ngang
                hNew = calculateHeuristicValue(next.x, next.y, end);
                fNew = gNew + hNew;

                if (details[next.x][next.y].f > fNew || details[next.x][next.y].f == FLT_MAX) {

                    q.push({ fNew, next });
                    details[next.x][next.y].g = gNew;
                    details[next.x][next.y].f = fNew;
                    details[next.x][next.y].h = hNew;
                    Game::widget->_gridRects[next.x][next.y]->_parentOnBFS = current.second;

                    if (_timeOnFrame)
                    {
                        Game::widget->_gridRects[next.x][next.y]->setStepOnBFS();
                        _stepsOnBFS.push_back(next);
                        Game::widget->update();
                        delay(_timeOnFrame);
                    }
                    if (next.x == end.x && next.y == end.y)
                    {
                        vector<Point> path;
                        while (!(next.x == start.x && next.y == start.y))
                        {
                            path.push_back(Game::widget->_gridRects[next.x][next.y]->_parentOnBFS);
                            next = Game::widget->_gridRects[next.x][next.y]->_parentOnBFS;
                        }
                        clearStepsOnBFS(_stepsOnBFS);
                        return path;
                    }
                }


                if (!_isOnBFS)
                {
                    clearStepsOnBFS(_stepsOnBFS);
                    return {};
                }

            }
        }
    }
    clearStepsOnBFS(_stepsOnBFS);
    return {}; // Không tìm thấy đường đi
}

void Controller::setupController()
{
    QGridLayout* layout = new QGridLayout(this);
    vector<typeButton> buttons = {
        typeButton::UnBlocked,
        typeButton::Blocked,
        typeButton::Target,
        typeButton::Source,
        typeButton::Starting,
        typeButton::ClearPath,
        typeButton::Resize,
        typeButton::Reset,
        typeButton::ClearGrid,
        typeButton::StopFinding,
        typeButton::SetTimeOnFrame};

    for (auto row : buttons) {
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
    case typeButton::ClearGrid:
        this->setText("Clear Grid");
        this->setStyleSheet("background-color: white"); // Đặt màu nền cho nút
        break;
    case typeButton::StopFinding:
        this->setText("Stop Finding");
        this->setStyleSheet("background-color: white"); // Đặt màu nền cho nút
        break;
    case typeButton::SetTimeOnFrame:
        this->setText("Set Time On Frame");
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

    //std::cout << this->text().toStdString() << std::endl;
    _controller->_status = this->_type;
    if (_controller->_isOnBFS 
        && _controller->_status != typeButton::StopFinding 
        && _controller->_status != typeButton::ClearGrid
        && _controller->_status != typeButton::Blocked
        && _controller->_status != typeButton::UnBlocked
        && _controller->_status != typeButton::SetTimeOnFrame)
    {
        QMessageBox::information(this, "Message", "Dang trong trang thai tim kiem, an Stop Finding de thoat.");
        return;
    }
    if (_controller->_status == typeButton::Starting)
    {
        if (_controller->_source && _controller->_target && _controller->_source->isValid() && _controller->_target->isValid())
        {
            Game::widget->clearPath();
            _controller->_path = _controller->BFS();
            _controller->_isOnBFS = false;
            Game::widget->drawPath();

        }
        else
        {
            QMessageBox::information(this, "Message", "Starting: chon source va target de bat dau tim kiem.");
        }
      

    }
    else if (_controller->_status == typeButton::ClearPath)
    {
        Game::widget->clearPath();

    }
    else if (_controller->_status == typeButton::Reset)
    {

        _controller->_source = nullptr;
        _controller->_target = nullptr;
        _controller->_path.clear();
        Game::widget.reset();
        Game::widget = std::make_shared<GridWidget>(32, 32, &*Game::demo);
        Game::demo->launch();
        Game::widget->show();
        return;
    }
    else if (_controller->_status == typeButton::Resize)
    {
        if (_controller->_resize.isVisible())
        {
            _controller->_resize.close();
        }
        _controller->_resize.resize(200, 150);
        _controller->_resize.show();
    }
    else if (_controller->_status == typeButton::ClearGrid)
    {
        _controller->_isClearGrid = !_controller->_isClearGrid;
    }
    else if(_controller->_status == typeButton::StopFinding)
    {
        _controller->_isOnBFS = false;
    }
    else if (_controller->_status == typeButton::SetTimeOnFrame)
    {
        if (_controller->_setTimeOnFrame.isVisible())
        {
            _controller->_setTimeOnFrame.close();
        }
        _controller->_setTimeOnFrame.resize(200, 100);
        _controller->_setTimeOnFrame.show();
    }
    Game::widget->update();
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

    if (width >= 32 && width <= 256 && height >= 32 && height <= 256)
    {
        Game::controller->_source = nullptr;
        Game::controller->_target = nullptr;
        Game::controller->_path.clear();
        Game::widget.reset();
        Game::widget = std::make_shared<GridWidget>(width, height, &*Game::demo);
        Game::demo->launch();
        Game::widget->show();
    }
    this->close();
}

SetTimeWidget::SetTimeWidget(QWidget* parent)
    : QWidget(parent) {
    // Create labels and input fields
    QLabel* setTimeLabel = new QLabel("Time (0-1000) ms:", this);
    setTimeEdit = new QLineEdit(this);

    // Create the resize button
    setTimeButton = new QPushButton("SetTimeOnFrame", this);


    QDoubleValidator* intValidator = new QDoubleValidator(0.f, 1000.f, 3, this); // Adjust range as needed
    setTimeEdit->setValidator(intValidator);


    // Create layout for the input fields
    QHBoxLayout* setTimeLayout = new QHBoxLayout;
    setTimeLayout->addWidget(setTimeLabel);
    setTimeLayout->addWidget(setTimeEdit);


    // Create main layout
    QVBoxLayout* mainLayout = new QVBoxLayout(this);
    mainLayout->addLayout(setTimeLayout);
    mainLayout->addWidget(setTimeButton);

    // Set the main layout for the widget
    setLayout(mainLayout);

    // Connect the resize button click signal to the slot
    connect(setTimeButton, &QPushButton::clicked, this, &SetTimeWidget::onSetTimeButtonClicked);
}

void SetTimeWidget::onSetTimeButtonClicked() {
    // Get the input values
    auto time = setTimeEdit->text().toDouble();

    // Resize the widget

    if (time >= 0 && time <= 1000 )
    {
        Game::controller->_timeOnFrame = time;
    }
    this->close();
}
