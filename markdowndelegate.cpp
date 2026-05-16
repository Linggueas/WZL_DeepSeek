#include "markdowndelegate.h"
#include <QTextDocument>
#include <QPainter>
#include <QAbstractTextDocumentLayout>
MarkdownDelegate::MarkdownDelegate() : m_fontSize(15) {}

void MarkdownDelegate::setFontSize(int size)
{
    if (size < 8) size = 8;
    if (size > 48) size = 48;
    m_fontSize = size;
    m_sizeCache.clear();
}

int MarkdownDelegate::fontSize() const
{
    return m_fontSize;
}

void MarkdownDelegate::invalidateCache()
{
    m_sizeCache.clear();
}

QSize MarkdownDelegate::sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    QString mdText = index.data(Qt::DisplayRole).toString();
    if (mdText.isEmpty())
        return QStyledItemDelegate::sizeHint(option, index);

    uint hash = qHash(mdText);
    if (m_sizeCache.contains(hash))
        return m_sizeCache.value(hash);

    QTextDocument doc;
    QFont font = doc.defaultFont();
    font.setPointSize(m_fontSize);
    doc.setDefaultFont(font);
    doc.setMarkdown(mdText);
    doc.setTextWidth(option.rect.width());

    QSize sz(option.rect.width(), doc.size().height());
    m_sizeCache.insert(hash, sz);
    return sz;
}

void MarkdownDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    QString mdText = index.data(Qt::DisplayRole).toString();
    if (mdText.isEmpty())
        return QStyledItemDelegate::paint(painter, option, index);

    painter->save();

    QTextDocument doc;
    QFont font = doc.defaultFont();
    font.setPointSize(m_fontSize);
    doc.setDefaultFont(font);
    doc.setMarkdown(mdText);
    doc.setTextWidth(option.rect.width());

    painter->translate(option.rect.topLeft());
    QAbstractTextDocumentLayout::PaintContext ctx;
    ctx.palette = option.palette;
    doc.documentLayout()->draw(painter, ctx);

    painter->restore();
}
