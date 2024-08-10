#include "Map.h"

GridWidget::GridWidget(int maxX, int maxY, QWidget* parent)
    : QWidget(parent),
      _maxX(maxX),
      _maxY(maxY)
{
    if (_maxY > 60 || _maxX > 100)
    {
        _cellSize = 10;
    }
    if (_maxY > 100 || _maxX > 180)
    {
        _cellSize = 5;
    }
    if (_maxY > 150)
    {
        _cellSize = 4;
    }

    _gridSizeRow = _cellSize * (_maxX);
    _gridSizeCol = _cellSize * (_maxY);
    setFixedSize(_gridSizeCol, _gridSizeRow);
    setupGrid();
}

void GridWidget::mousePressEvent(QMouseEvent* event)
{
    
    if (event->button() == Qt::LeftButton) {
        _isDragging = true;
        //_lastMousePosition = event->pos();
    }
    int x = event->pos().x() / _cellSize + 1;
    int y = event->pos().y() / _cellSize + 1;

    if (event->pos().x() < _gridSizeCol && event->pos().y() < _gridSizeRow && event->pos().y() > 0 && event->pos().x() > 0) {
        _clickedCell = QPoint(y, x);
        std::cout << "Cell clicked at position (" << y << ", " << x << ")" << std::endl;
        gridUpdate(event);
    }
    
}

void GridWidget::mouseMoveEvent(QMouseEvent* event)
{
    if (_isDragging)
    {
        int x = event->pos().x() / _cellSize + 1;
        int y = event->pos().y() / _cellSize + 1;

        if (event->pos().x() < _gridSizeCol && event->pos().y() < _gridSizeRow && event->pos().y() > 0 && event->pos().x() > 0) {
            _clickedCell = QPoint(y, x);
            std::cout << "Cell dragged at position (" << y << ", " << x << ")" << std::endl;
            gridUpdate(event);
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
    QPainter painter1(this);

    painter1.drawPixmap(0, 0, _pixmap); // Vẽ lại pixmap hiện tại
    QPainter painter(&_pixmap);
    //if (_isFirst)
    //{
    //    int cnt = 0;
    //    for (int i = 1; i <= _maxX; ++i) {
    //        for (int j = 1; j <= _maxY; ++j) {
    //            cout << "paintEvent " << ++cnt << endl;
    //            switch (_gridRects[i][j]->_type)
    //            {
    //            case Cell::typeCell::Blocked:
    //                painter.setPen(Qt::gray);
    //                break;
    //            case Cell::typeCell::UnBlocked:
    //                painter.setPen(Qt::white);
    //                break;
    //            case Cell::typeCell::Target:
    //                painter.setPen(Qt::red);
    //                break;
    //            case Cell::typeCell::Source:
    //                painter.setPen(Qt::green);
    //                break;
    //            case Cell::typeCell::Path:
    //                painter.setPen(Qt::cyan);
    //                break;
    //            default:
    //                break;
    //            }


    //            painter.fillRect(*_gridRects[i][j], painter.pen().color());
    //            if (!controller->_isClearGrid)
    //            {
    //                QPen blackPen(Qt::black);
    //                painter.setPen(blackPen);
    //                painter.drawRect(*_gridRects[i][j]);
    //            }

    //        }
    //    }
    //    _isFirst = false;
    //}
    //else
    //{
        if (_clickedCell.x() >= 1 && _clickedCell.x() <= _maxX && _clickedCell.y() >= 1 && _clickedCell.y() <= _maxY)
        {
            switch (_gridRects[_clickedCell.x()][_clickedCell.y()]->_type)
            {
            case Cell::typeCell::Blocked:
                painter.setPen(Qt::gray);
                break;
            case Cell::typeCell::UnBlocked:
                painter.setPen(Qt::white);
                break;
            case Cell::typeCell::Target:
                painter.setPen(Qt::red);
                break;
            case Cell::typeCell::Source:
                painter.setPen(Qt::green);
                break;
            case Cell::typeCell::Path:
                painter.setPen(Qt::cyan);
                break;
            default:
                break;
            }

            painter.fillRect(*_gridRects[_clickedCell.x()][_clickedCell.y()], painter.pen().color());
            QPen blackPen(Qt::black);
            painter.setPen(blackPen);
            painter.drawRect(*_gridRects[_clickedCell.x()][_clickedCell.y()]);
        }
    //}

    cout << "paintEvent " << endl;
}

void GridWidget::gridUpdate(QMouseEvent* e)
{
    if (e->button() == Qt::RightButton)
    {
        _gridRects[_clickedCell.x()][_clickedCell.y()]->setUnBlocked();
    }
    else if (e->button() == Qt::LeftButton)
    {
        switch (controller->_status)
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

        std::cout << "Cell: " << _gridRects[_clickedCell.x()][_clickedCell.y()]->_type << " " << _gridRects[_clickedCell.x()][_clickedCell.y()]->_y << " " << _gridRects[_clickedCell.x()][_clickedCell.y()]->_x << " map value: " << std::endl;
    }
    else if (_isDragging)
    {
        switch (controller->_status)
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
        std::cout << "Cell: " << _gridRects[_clickedCell.x()][_clickedCell.y()]->_type << " " << _gridRects[_clickedCell.x()][_clickedCell.y()]->_x << " " << _gridRects[_clickedCell.x()][_clickedCell.y()]->_y << " map value: " << std::endl;
    }
    //update(*(_gridRects[_clickedCell.x()][_clickedCell.y()].get()));
    update();
}

void GridWidget::drawInitialGrid(QPainter& painter)
{
    int cnt = 0;
    for (int i = 1; i <= _maxX; ++i) {
        for (int j = 1; j <= _maxY; ++j) {
            cout << "paintEvent Initial " << ++cnt << endl;
            switch (_gridRects[i][j]->_type)
            {
            case Cell::typeCell::Blocked:
                painter.setPen(Qt::gray);
                break;
            case Cell::typeCell::UnBlocked:
                painter.setPen(Qt::white);
                break;
            case Cell::typeCell::Target:
                painter.setPen(Qt::red);
                break;
            case Cell::typeCell::Source:
                painter.setPen(Qt::green);
                break;
            case Cell::typeCell::Path:
                painter.setPen(Qt::cyan);
                break;
            default:
                break;
            }


            painter.fillRect(*_gridRects[i][j], painter.pen().color());
            if (!controller->_isClearGrid)
            {
                QPen blackPen(Qt::black);
                painter.setPen(blackPen);
                painter.drawRect(*_gridRects[i][j]);
            }
        }
    }
}


void GridWidget::setupGrid()
{
    _gridRects.resize(_maxX + 5);

    for (int row = 1; row <= _maxX; ++row) {
        _gridRects[row].resize(_maxY + 5);
        for (int col = 1; col <= _maxY; ++col) {

            auto cell = std::make_shared<Cell>();
            cell->setLocation(row, col);
            cell->setX(_cellSize * (col - 1));
            cell->setY(_cellSize * (row - 1));
            cell->setSize(QSize(_cellSize, _cellSize));
            _gridRects[row][col] = cell;
        }
    }
    // Khởi tạo pixmap với kích thước của widget
    _pixmap = QPixmap(size());
    _pixmap.fill(Qt::transparent); // Đảm bảo rằng pixmap ban đầu là trong suốt

    // Vẽ toàn bộ các hình vuông ban đầu lên pixmap
    QPainter painter(&_pixmap);
    drawInitialGrid(painter);

}

void GridWidget::drawPath()
{
    if (controller->_path.size())
    {
        for (auto cell : controller->_path)
        {
            if(this->_gridRects[cell.x][cell.y]->_type == Cell::typeCell::UnBlocked)
                this->_gridRects[cell.x][cell.y]->_type = Cell::typeCell::Path;
        }

    }
}


void Cell::setLocation(int x, int y)
{
    _x = x;
    _y = y;
}

void Cell::setSource()
{
    if (this->_type == typeCell::Target) return;
    if (controller->_source)
    {
        controller->_source->_type = typeCell::UnBlocked;
    }
    controller->_source = this;
    controller->_source->_type = typeCell::Source;

}

void Cell::setTarget()
{
    if (this->_type == typeCell::Source) return;
    if (controller->_target)
    {
        controller->_target->_type = typeCell::UnBlocked;
    }
    controller->_target = this;
    controller->_target->_type = typeCell::Target;
}

void Cell::setBlocked()
{
    if (this->_type != typeCell::Source && this->_type != typeCell::Target)
    {
        _type = typeCell::Blocked;
    }

}

void Cell::setUnBlocked()
{
    if (this->_type != typeCell::Source && this->_type != typeCell::Target)
    {
        _type = typeCell::UnBlocked;
    }
}
