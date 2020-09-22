#include "mygraphicsview.h"

MyGraphicsView::MyGraphicsView(QWidget* widget)
{
  //QGraphicsView::QGraphicsView(widget);
}

void MyGraphicsView::mousePressEvent(QMouseEvent *event)
{
    emit clicked(1);
}
