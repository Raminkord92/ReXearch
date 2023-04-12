#include <Headers/Views/axtableview.h>

#include <QMouseEvent>
#include <QDebug>

AXTableView::AXTableView(QObject *parent)
{
    Q_UNUSED(parent);
    this->setMouseTracking(true);
}

void AXTableView::mouseMoveEvent(QMouseEvent *event)
{
    QModelIndex index = indexAt(event->pos());
    if (!index.isValid())
        return;

    if (index.column() == 4)
    {
        this->setCursor(Qt::CursorShape::PointingHandCursor);

    }
    else
    {
        this->unsetCursor();
    }
}

void AXTableView::mousePressEvent(QMouseEvent *event)
{
    QModelIndex index = indexAt(event->pos());
    if (!index.isValid())
        return;
    if (event->button() == Qt::RightButton)
    {
        return;
    }
    else
    {
        qDebug() << "SALAM LEFT CLICK";
        setSelectionMode(QAbstractItemView::SingleSelection);
        setSelectionBehavior(QAbstractItemView::SelectRows);
        setCurrentIndex(index);

    }

    return;
}

void AXTableView::mouseDoubleClickEvent(QMouseEvent *event)
{
    QModelIndex index = indexAt(event->pos());
    if (!index.isValid())
        return;
    emitDoubleClicked(index);
}



