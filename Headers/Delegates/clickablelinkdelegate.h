#ifndef CLICKABLELINKDELEGATE_H
#define CLICKABLELINKDELEGATE_H

#include <QAbstractItemDelegate>
#include <QObject>
#include <QStyledItemDelegate>
#include <Headers/clickablelabel.h>
#include <QLabel>
#include <QTableView>
#include <QPushButton>

class ClickableLinkDelegate : public QStyledItemDelegate
{
    Q_OBJECT
public:
    explicit ClickableLinkDelegate(QObject *parent = nullptr);

    // QAbstractItemDelegate interface

signals:
    void clicked(QString text);

    // QAbstractItemDelegate interface
public:
    void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const;
    QSize sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const;
    void updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex &index) const;
    bool editorEvent(QEvent *event, QAbstractItemModel *model, const QStyleOptionViewItem &option, const QModelIndex &index);

private:
    QStyle::State  _state;

};

#endif // CLICKABLELINKDELEGATE_H
