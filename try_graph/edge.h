#ifndef EDGE_H
#define EDGE_H

#include <QGraphicsItem>
#include "graph.h"
class graphVertex;

class Edge : public QGraphicsItem
{
public:
    Edge(graphVertex *sourceVert, graphVertex *destVert);

    ~Edge() override;
    graphVertex *sourceNode() const;
    graphVertex *destNode() const;
    void setContent(int);
    void adjust();
    void setGraph(Graph*);
    //enum { Type = UserType + 2 };
    //int type() const override { return Type; }

protected:
    QRectF boundingRect() const override;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;
    void mousePressEvent(QGraphicsSceneMouseEvent *event) override;
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event) override;

private:
    graphVertex *source, *dest;
    bool _pressed;

    QPointF sourcePoint;
    QPointF destPoint;
    qreal arrowSize;
    Graph *graph;
    QGraphicsItemGroup drawing;

    QString content = "weight";
    //QString name =

};


#endif // EDGE_H
