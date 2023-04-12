#include <Headers/Views/axtableview.h>

#include <QMouseEvent>
#include <QDebug>
#include "Headers/Common.h"

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

    if (index.column() == (int)SearchTblColEnum::Address)
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
        if (index.column() == (int)SearchTblColEnum::Address)
            emit onLinkCLicked(index.model()->data(index).toString());
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



