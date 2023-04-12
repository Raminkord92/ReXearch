#include "axlistview.h"

AXListView::AXListView(QObject *parent)
{
    Q_UNUSED(parent);
    this->setMouseTracking(true);

}

QModelIndex AXListView::hoveredIndex() const
{
    return m_hoveredIndex;
}

void AXListView::setHoveredIndex(const QModelIndex &newHoveredIndex)
{
    m_hoveredIndex = newHoveredIndex;
}
