#ifndef MYGRAPHICSVIEW_H
#define MYGRAPHICSVIEW_H

#include <QGraphicsScene>
#include <QGraphicsView>

class MyGraphicsView : public QGraphicsView
{
    Q_OBJECT
public:
    MyGraphicsView(QWidget* widget);
    void mousePressEvent(QMouseEvent *event);

signals:
    void clicked(int val);
};

#endif // MYGRAPHICSVIEW_H
