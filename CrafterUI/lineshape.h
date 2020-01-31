#ifndef LINESHAPE_H
#define LINESHAPE_H

#include <QQuickPaintedItem>
#include <vector>
#include <QLineF>

namespace crafter {

struct LineConnection {
    std::pair<double, double> start;
    std::pair<double, double> end;
    bool highlighted = false;
    std::pair<QPointF, QPointF> toQPointF () const;
    QLineF toQLineF() const ;
    friend std::ostream& operator<<(std::ostream&, const LineConnection&);
};

class LineShape : public QQuickPaintedItem
{
    Q_OBJECT
    Q_PROPERTY(QColor base READ base WRITE setBase NOTIFY colorChanged)
    Q_PROPERTY(QColor highlight READ highlight WRITE setHighlight NOTIFY colorChanged)
    Q_PROPERTY(int lineWidth READ lineWidth WRITE setLineWidth NOTIFY colorChanged);
public:
    LineShape(QQuickItem *parent = 0);

    void paint(QPainter *painter);
    void setLines(const std::vector<LineConnection>);

    QColor base() const;
    void setBase(const QColor& color);
    QColor highlight() const;
    void setHighlight(const QColor& color);
    int lineWidth() const;
    void setLineWidth(const int width);


signals:
    void colorChanged();

private:
    std::vector<LineConnection> lines;
    std::vector<LineConnection> highlighted;
    void add_lines(void);
    QColor _base = "black";
    QColor _highlight = "pink";
    int _lineWidth = 6;
};

}


#endif // LINESHAPE_H
