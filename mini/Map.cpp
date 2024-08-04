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

    _gridSizeRow = _cellSize * (_maxX + 1);
    _gridSizeCol = _cellSize * (_maxY + 1);
    setFixedSize(_gridSizeRow, _gridSizeCol);
    setupGrid();
}

void GridWidget::mousePressEvent(QMouseEvent* event)
{
    
        int x = event->pos().x() / _cellSize;
        int y = event->pos().y() / _cellSize;

        if (event->pos().x() < _gridSizeRow && event->pos().y() < _gridSizeCol && event->pos().y() > _cellSize && event->pos().x() > _cellSize) {
            _clickedCell = QPoint(x, y);
            std::cout << "Cell clicked at position (" << x << ", " << y << ")" << std::endl;
            gridUpdate(event);
        }
    
}

void GridWidget::paintEvent(QPaintEvent* event)
{
    QPainter painter(this);

    for (int i = 1; i <= _maxX; ++i) {
        for (int j = 1; j <= _maxY; ++j) {
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
                if (_cellSize != 4)
                {
                QPen blackPen(Qt::black);
                painter.setPen(blackPen);
                painter.drawRect(*_gridRects[i][j]);
                }

        }
    }
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

        std::cout << "Cell: " << _gridRects[_clickedCell.x()][_clickedCell.y()]->_type << " " << _gridRects[_clickedCell.x()][_clickedCell.y()]->_x << " " << _gridRects[_clickedCell.x()][_clickedCell.y()]->_y << " map value: " << std::endl;
    }
    update();
}

void GridWidget::setupGrid()
{
    _gridRects.resize(_maxX + 5);

    for (int row = 1; row <= _maxX; ++row) {
        _gridRects[row].resize(_maxY + 5);
        for (int col = 1; col <= _maxY; ++col) {

            auto cell = std::make_shared<Cell>();
            cell->setLocation(row, col);
            cell->setX(_cellSize * row);
            cell->setY(_cellSize * col);
            cell->setSize(QSize(_cellSize, _cellSize));
            _gridRects[row][col] = cell;
        }
    }

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
