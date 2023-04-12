#include "Headers/Delegates/clickablelinkdelegate.h"
#include "qapplication.h"
#include <QEvent>
#include <QPainter>
#include <QStyleOptionViewItem>
#include <QPushButton>

ClickableLinkDelegate::ClickableLinkDelegate(QObject *parent)
    : QStyledItemDelegate{parent}
{

}

QSize ClickableLinkDelegate::sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    return QStyledItemDelegate::sizeHint(option, index);
}

void ClickableLinkDelegate::updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    Q_UNUSED(index);
    editor->setGeometry(option.rect);
}

bool ClickableLinkDelegate::editorEvent(QEvent *event, QAbstractItemModel *model, const QStyleOptionViewItem &option, const QModelIndex &index)
{
    Q_UNUSED(option);
    Q_UNUSED(model);
    if (event->type() == QEvent::MouseButtonRelease)
    {
        emit clicked(index.data(Qt::DisplayRole).toString());
        return true;
    }
    else
    {
        return false;
    }
}


void ClickableLinkDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    QStyleOptionViewItem opt = option;
     initStyleOption(&opt, index);

     if (opt.text.contains("http://") || opt.text.contains("https://"))
     {
         opt.text = "Link";
         opt.textElideMode = Qt::ElideNone;
         opt.font.setUnderline(true);
         const QWidget *widget = opt.widget;
         opt.palette.setColor(QPalette::Text, QColor(Qt::blue));
         QStyle *style = widget ? widget->style() : QApplication::style();
         style->drawControl(QStyle::CE_ItemViewItem, &opt, painter, widget);
        return;
     }

     QStyledItemDelegate::paint(painter, opt, index);
}
