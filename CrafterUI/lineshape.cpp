#include "lineshape.h"
#include <QPen>
#include <QPainter>
#include <iostream>
#include <QPainterPath>

namespace crafter {

LineShape::LineShape(QQuickItem *parent) : QQuickPaintedItem(parent), _base{"black"}, _highlight{"pink"} {
    add_lines();
}

void LineShape::LineShape::add_lines(void) {
    lines = {{{0, 0}, {100, 50}}, {{200, 300}, {500, 500}}};
}

void LineShape::paint(QPainter *painter) {

    QPen base_pen(_base, _lineWidth), highlight_pen(_highlight, _lineWidth);
    painter->setRenderHints(QPainter::Antialiasing, true);

    auto count = 0;
    for (const auto& connection : lines) {
        if (connection.highlighted) {
            painter->setPen(highlight_pen);
        } else {
            painter->setPen(base_pen);
        }
        painter->drawLine(connection.toQLineF());
        count++;
    }
    std::cout << "Painted " << count << " lines\n";
}

void LineShape::setLines(std::vector<LineConnection> new_lines) {
    lines = new_lines;
    update();
}

QColor LineShape::base() const {
    return _base;
}

void LineShape::setBase(const QColor& color) {
    if (color != _base) {
        _base = color;
        update();
        emit colorChanged();
    }

}
QColor LineShape::highlight() const {
    return _highlight;
}

void LineShape::setHighlight(const QColor& color) {
    if (color != _highlight) {
        _highlight = color;
        update();
        emit colorChanged();
    }

}

int LineShape::lineWidth() const {
    return _lineWidth;
}

void LineShape::setLineWidth(const int width) {
    if (width != _lineWidth) {
        _lineWidth = width;
        update();
        emit colorChanged();
    }
}

std::pair<QPointF, QPointF> LineConnection::toQPointF () const {
    return {QPointF(start.first, start.second), QPointF(end.first, end.second)};
}

QLineF LineConnection::toQLineF() const {
    const auto& points = toQPointF();
    return QLineF(points.first, points.second);
}

std::ostream& operator<<(std::ostream& out, const LineConnection& connection) {
    return out << "Start: (" << connection.start.first << ", " << connection.start.second << ") Second: (" << connection.end.first << ", " << connection.end.second << ")\n";
}

}
