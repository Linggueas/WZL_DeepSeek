#ifndef MARKDOWNDELEGATE_H
#define MARKDOWNDELEGATE_H

#include <QStyledItemDelegate>
#include <QHash>
#include <QSize>

class MarkdownDelegate : public QStyledItemDelegate
{
    Q_OBJECT
public:
    MarkdownDelegate();
    void paint(QPainter *painter,
               const QStyleOptionViewItem &option, const QModelIndex &index) const override;
    QSize sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const override;
    void invalidateCache();
    void setFontSize(int size);
    int fontSize() const;

private:
    mutable QHash<uint, QSize> m_sizeCache;
    int m_fontSize;
};

#endif // MARKDOWNDELEGATE_H
