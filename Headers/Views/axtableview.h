#ifndef AXTABLEVIEW_H
#define AXTABLEVIEW_H

#include <QTableView>
#include <QObject>

class AXTableView : public QTableView
{
    Q_OBJECT
public:
    AXTableView( QObject * parent = 0);

    // QWidget interface
protected:
    void mouseMoveEvent(QMouseEvent *event);
    void mousePressEvent(QMouseEvent *event) override;
    void mouseDoubleClickEvent(QMouseEvent *event) override;
signals:
    void emitDoubleClicked(const QModelIndex& index);
    void onLinkCLicked(const QString& text);
};

#endif // AXTABLEVIEW_H
