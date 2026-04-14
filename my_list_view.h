#ifndef MY_LIST_VIEW_H
#define MY_LIST_VIEW_H

#include <QListView>

class my_list_view : public QListView
{
    Q_OBJECT
public:
    my_list_view(QWidget *parent = nullptr);
    void mousePressEvent(QMouseEvent *event)override;
signals:
    void sign_delete();
};

#endif // MY_LIST_VIEW_H
