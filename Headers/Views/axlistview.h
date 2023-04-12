#ifndef AXLISTVIEW_H
#define AXLISTVIEW_H

#include <QListView>
#include <QMouseEvent>
#include <QObject>

class AXListView : public QListView
{
    Q_OBJECT
public:
    explicit AXListView(QObject *parent = nullptr);

    QModelIndex hoveredIndex() const;
    void setHoveredIndex(const QModelIndex &newHoveredIndex);

protected:
    void mousePressEvent(QMouseEvent *event) override
    {
        QModelIndex index = indexAt(event->pos());
        if (!index.isValid())
        {
            setSelectionMode(SelectionMode::NoSelection);
            return;
        }
        if(event->button() == Qt::RightButton)
        {
            setHoveredIndex(index);
            emit contextMenuRequested(event->pos());
            return;
        }

         setSelectionMode(SelectionMode::SingleSelection);
         setCurrentIndex(index);
    }

    void mouseDoubleClickEvent(QMouseEvent *event) override
    {
        QModelIndex index = indexAt(event->pos());
        if (index.isValid())
        {
            setSelectionMode(SelectionMode::SingleSelection);

            qDebug() << "index.isValid()";
            emit dblClicked(index);
        }
    }

signals:
    void dblClicked(const QModelIndex& index);
    void contextMenuRequested(const QPoint& point);
private:
    QModelIndex m_hoveredIndex;

};

#endif // AXLISTVIEW_H
