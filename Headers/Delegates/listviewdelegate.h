#ifndef LISTVIEWDELEGATE_H
#define LISTVIEWDELEGATE_H

#include <QObject>
#include <QItemDelegate>
#include <QEvent>
#include <QItemSelectionModel>
#include <QMouseEvent>
// First, create the delegate class with a member variable for the selection model

class MyDelegate : public QItemDelegate {
public:
    MyDelegate(QObject* parent = nullptr) : QItemDelegate(parent) {}

    void setSelectionModel(QItemSelectionModel* selectionModel) {
        m_selectionModel = selectionModel;
    }

    bool editorEvent(QEvent* event, QAbstractItemModel* model, const QStyleOptionViewItem& option, const QModelIndex& index) override {
        if (!m_selectionModel) {
            return false;
        }

        if (event->type() == QEvent::MouseButtonPress && static_cast<QMouseEvent*>(event)->button() == Qt::LeftButton) {
            m_selectionModel->setCurrentIndex(index, QItemSelectionModel::SelectCurrent);
            return true;
        }

        return QItemDelegate::editorEvent(event, model, option, index);
    }

private:
    QItemSelectionModel* m_selectionModel = nullptr;
};


#endif // LISTVIEWDELEGATE_H
