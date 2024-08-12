#include "Map.h"

GridWidget::GridWidget(int maxX, int maxY, QWidget* parent)
    : QWidget(parent),
      _maxRow(maxX),
      _maxCol(maxY)
{
    //if (_maxCol > 60 || _maxRow > 100)
    //{
    //    _cellSizeY = _cellSizeX = 10;
    //}
    //if (_maxCol > 100 || _maxRow > 180)
    //{
    //    _cellSizeY = _cellSizeX = 5;
    //}
    //if (_maxCol > 150)
    //{
    //    _cellSizeY = _cellSizeX = 4;
    //}

    _gridSizeRow = _cellSizeY * (_maxRow);
    _gridSizeCol = _cellSizeX * (_maxCol);
    setBaseSize(_gridSizeCol, _gridSizeRow);
    resize(_gridSizeCol, _gridSizeRow);
    setupInitialMap();
}

void GridWidget::mousePressEvent(QMouseEvent* event)
{
    
    if (event->button() == Qt::LeftButton) {
        _isDragging = true;
        //_lastMousePosition = event->pos();
    }
    int x = event->pos().x() / _cellSizeX + 1;
    int y = event->pos().y() / _cellSizeY + 1;

    if (event->pos().x() < _gridSizeCol && event->pos().y() < _gridSizeRow && event->pos().y() > 0 && event->pos().x() > 0) {
        _clickedCell = QPoint(y, x);
        //std::cout << "Cell clicked at position (" << y << ", " << x << ")" << std::endl;
        mapUpdate(event);
    }
    
}

void GridWidget::mouseMoveEvent(QMouseEvent* event)
{
    if (_isDragging)
    {
        int x = event->pos().x() / _cellSizeX + 1;
        int y = event->pos().y() / _cellSizeY + 1;

        if (event->pos().x() < _gridSizeCol && event->pos().y() < _gridSizeRow && event->pos().y() > 0 && event->pos().x() > 0) {
            _clickedCell = QPoint(y, x);
            //std::cout << "Cell dragged at position (" << y << ", " << x << ")" << std::endl;
            mapUpdate(event);
        }
    }
}

void GridWidget::mouseReleaseEvent(QMouseEvent* event)
{
    if (event->button() == Qt::LeftButton) {
        _isDragging = false;
    }
}

void GridWidget::paintEvent(QPaintEvent* event)
{
    QPainter painter(this);

    painter.drawPixmap(0, 0, _map); // Vẽ lại pixmap hiện tại
    if (!Game::controller->_isClearGrid)
    {
        painter.drawPixmap(0, 0, _grid);
    }

    //cout << "paintEvent " << endl;
}

void GridWidget::resizeEvent(QResizeEvent* event)
{
    auto Hmap = (Game::demo->height());
    auto Wmap = (width());

    _cellSizeX = Wmap / _maxCol;
    _cellSizeY = Hmap / _maxRow;
    _gridSizeRow = _cellSizeY * (_maxRow);
    _gridSizeCol = _cellSizeX * (_maxCol);
    resize(_gridSizeCol, _gridSizeRow);
    _map = QPixmap(size());
    _map.fill(Qt::transparent); // Đảm bảo rằng pixmap ban đầu là trong suốt

    _grid = QPixmap(size());
    _grid.fill(Qt::transparent);

    QPainter paintermap(&_map);
    QPainter paintergrid(&_grid);

    for (int row = 1; row <= _maxRow; ++row) {
        for (int col = 1; col <= _maxCol; ++col) {

            _gridRects[row][col]->setX(_cellSizeX * (col - 1));
            _gridRects[row][col]->setY(_cellSizeY * (row - 1));
            _gridRects[row][col]->setSize(QSize(_cellSizeX, _cellSizeY));

            drawMapAfterResize(paintermap, paintergrid, row, col);
        }
    }

    //QScreen* screen = QGuiApplication::primaryScreen();
    //auto widthScreen = screen->availableGeometry().width();
    //if (widthScreen - Game::widget->width() > Game::controller->width() + 20 && Game::widget->x() - Game::controller->width() > 20)
    //{
    //    Game::controller->move(Game::widget->x() - Game::controller->width() - 20, Game::widget->y());
    //}
}

void GridWidget::mapUpdate(QMouseEvent* e)
{
    if (_clickedCell.x() >= 1 && _clickedCell.x() <= _maxRow && _clickedCell.y() >= 1 && _clickedCell.y() <= _maxCol)
    {
        if (e->button() == Qt::RightButton)
        {
            _gridRects[_clickedCell.x()][_clickedCell.y()]->setUnBlocked();
        }
        else if (e->button() == Qt::LeftButton)
        {
            switch (Game::controller->_status)
            {
            case typeButton::Blocked:
                _gridRects[_clickedCell.x()][_clickedCell.y()]->setBlocked();
                break;
            case typeButton::UnBlocked:
                _gridRects[_clickedCell.x()][_clickedCell.y()]->setUnBlocked();
                break;
            case typeButton::Source:
                _gridRects[_clickedCell.x()][_clickedCell.y()]->setSource();
                break;
            case typeButton::Target:
                _gridRects[_clickedCell.x()][_clickedCell.y()]->setTarget();
                break;
            default:
                break;
            }

            //std::cout << "Cell: " << _gridRects[_clickedCell.x()][_clickedCell.y()]->_type << " " << _gridRects[_clickedCell.x()][_clickedCell.y()]->_y << " " << _gridRects[_clickedCell.x()][_clickedCell.y()]->_x << " map value: " << std::endl;
        }
        else if (_isDragging)
        {
            switch (Game::controller->_status)
            {
            case typeButton::Blocked:
                _gridRects[_clickedCell.x()][_clickedCell.y()]->setBlocked();
                break;
            case typeButton::UnBlocked:
                _gridRects[_clickedCell.x()][_clickedCell.y()]->setUnBlocked();
                break;
            default:
                break;
            }
            //std::cout << "Cell: " << _gridRects[_clickedCell.x()][_clickedCell.y()]->_type << " " << _gridRects[_clickedCell.x()][_clickedCell.y()]->_x << " " << _gridRects[_clickedCell.x()][_clickedCell.y()]->_y << " map value: " << std::endl;
        }
        //update(*(_gridRects[_clickedCell.x()][_clickedCell.y()].get()));
        update();
    }

}

void GridWidget::drawInitialMap(QPainter& paintermap, QPainter& paintergrid, int i ,int j)
{
            paintermap.setPen(Qt::white);
            paintergrid.setPen(Qt::black);
            paintermap.fillRect(*_gridRects[i][j], paintermap.pen().color());
            paintergrid.drawRect(*_gridRects[i][j]);
            paintergrid.drawLine(x(), height() + y(), x() + width(), y() + height());
            paintergrid.drawLine(x() + width(), y(), x() + width(), height() + y());
}

void GridWidget::drawMapAfterResize(QPainter& paintermap, QPainter& paintergrid, int i, int j)
{
    //int cnt = 0;
    //for (int i = 1; i <= _maxX; ++i) {
    //    for (int j = 1; j <= _maxY; ++j) {
            //cout << "paintEvent Initial " << ++cnt << endl;
    switch (_gridRects[i][j]->_type)
    {
    case Cell::typeCell::Blocked:
        paintermap.setPen(Qt::gray);
        break;
    case Cell::typeCell::UnBlocked:
        paintermap.setPen(Qt::white);
        break;
    case Cell::typeCell::Target:
        paintermap.setPen(Qt::red);
        break;
    case Cell::typeCell::Source:
        paintermap.setPen(Qt::green);
        break;
    case Cell::typeCell::Path:
        paintermap.setPen(Qt::cyan);
        break;
    case Cell::typeCell::StepOnBFS:
        paintermap.setPen(Qt::yellow);
        break;
    default:
        break;
    }

    //paintermap.setPen(Qt::white);
    paintergrid.setPen(Qt::black);
    paintermap.fillRect(*_gridRects[i][j], paintermap.pen().color());
    paintergrid.drawRect(*_gridRects[i][j]);
    paintergrid.drawLine(x(), height() + y(), x() + width(), y() + height());
    paintergrid.drawLine(x() + width(), y(), x() + width(), height() + y());
    //    }
    //}
}

void GridWidget::setupInitialMap()
{

    _map = QPixmap(size());
    _map.fill(Qt::transparent); 

    _grid = QPixmap(size());
    _grid.fill(Qt::transparent);

    QPainter paintermap(&_map);
    QPainter paintergrid(&_grid);


    _gridRects.resize(_maxRow + 5);

    for (int row = 1; row <= _maxRow; ++row) {
        _gridRects[row].resize(_maxCol + 5);
        for (int col = 1; col <= _maxCol; ++col) {

            auto cell = std::make_shared<Cell>();
            cell->setLocation(row, col);
            cell->setX(_cellSizeX * (col - 1));
            cell->setY(_cellSizeY * (row - 1));
            cell->setSize(QSize(_cellSizeX, _cellSizeY));
            _gridRects[row][col] = cell;

            drawInitialMap(paintermap, paintergrid, row, col);
        }
    }


}

void GridWidget::drawPath()
{

    QPainter paintermap(&Game::widget->_map);
    if (Game::controller->_path.size())
    {
        for (auto cell : Game::controller->_path)
        {
            if (this->_gridRects[cell.x][cell.y]->_type == Cell::typeCell::UnBlocked || this->_gridRects[cell.x][cell.y]->_type == Cell::typeCell::StepOnBFS)
            {
                this->_gridRects[cell.x][cell.y]->_type = Cell::typeCell::Path;
                paintermap.setPen(Qt::cyan);
                paintermap.fillRect(*_gridRects[cell.x][cell.y], paintermap.pen().color());
            }

        }

    }
}

void GridWidget::clearPath()
{
    if (Game::controller->_path.size())
    {
        for (auto cell : Game::controller->_path)
        {
            if (this->_gridRects[cell.x][cell.y]->_type == Cell::typeCell::Path)
            {
                this->_gridRects[cell.x][cell.y]->setUnBlocked();
            }

        }
        Game::controller->_path.clear();
    }

}


void GridWidget::enterEvent(QEvent* event)
{
    QWidget::enterEvent(event);
    Game::controller->raise();

}


void Cell::setLocation(int x, int y)
{
    _x = x;
    _y = y;
}

void Cell::setSource()
{
    QPainter paintermap(&Game::widget->_map);
    if (this->_type == typeCell::Target) return;
    if (Game::controller->_source)
    {
        Game::controller->_source->_type = typeCell::UnBlocked;
        paintermap.setPen(Qt::white);
        paintermap.fillRect(*Game::controller->_source, paintermap.pen().color());
    }
    Game::controller->_source = this;
    Game::controller->_source->_type = typeCell::Source;
    paintermap.setPen(Qt::green);
    paintermap.fillRect(*this, paintermap.pen().color());

}

void Cell::setTarget()
{
    QPainter paintermap(&Game::widget->_map);
    if (this->_type == typeCell::Source) return;
    if (Game::controller->_target)
    {
        Game::controller->_target->_type = typeCell::UnBlocked;
        paintermap.setPen(Qt::white);
        paintermap.fillRect(*Game::controller->_target, paintermap.pen().color());
    }
    Game::controller->_target = this;
    Game::controller->_target->_type = typeCell::Target;
    paintermap.setPen(Qt::red);
    paintermap.fillRect(*this, paintermap.pen().color());
}

void Cell::setBlocked()
{
    QPainter paintermap(&Game::widget->_map);
    if (this->_type != typeCell::Source && this->_type != typeCell::Target)
    {
        _type = typeCell::Blocked;
        paintermap.setPen(Qt::gray);
        paintermap.fillRect(*this, paintermap.pen().color());
    }

}

void Cell::setUnBlocked()
{
    QPainter paintermap(&Game::widget->_map);
    if (this->_type != typeCell::Source && this->_type != typeCell::Target)
    {
        _type = typeCell::UnBlocked;
        paintermap.setPen(Qt::white);
        paintermap.fillRect(*this, paintermap.pen().color());
    }

}

void Cell::setStepOnBFS()
{
    QPainter paintermap(&Game::widget->_map);
    if (this->_type != typeCell::Source && this->_type != typeCell::Target)
    {
        _type = typeCell::StepOnBFS;
        paintermap.setPen(Qt::yellow);
        paintermap.fillRect(*this, paintermap.pen().color());
    }
}


Game::Game(QWidget * parent)
    : QWidget(parent) {

    this->resize(1000, 1000);
    layout1 = new QHBoxLayout(&*demo);
    layout2 = new QHBoxLayout(&*demo);
    mainLayout = new QHBoxLayout(&*demo);

    //QHBoxLayout* layout = new QHBoxLayout(this);
    //layout->addWidget(&*Game::widget);
    //layout->addWidget(&*Game::controller);

    //setLayout(layout);
}

