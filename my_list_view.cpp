#include "my_list_view.h"
#include<QMouseEvent>
my_list_view::my_list_view(QWidget *parent):QListView(parent) {}

void my_list_view::mousePressEvent(QMouseEvent *event)
{
    if(event->button() == Qt::RightButton)
    {
        emit sign_delete();
        return;
    }
    QListView::mousePressEvent(event);
}
