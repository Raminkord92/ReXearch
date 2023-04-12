#include "Headers/Models/axqstringlistmodel.h"
#include "qcolor.h"
#include <QDebug>
#include <QBrush>

AXQStringListModel::AXQStringListModel(QObject *parent)
    : QStringListModel{parent}
{

}

QVariant AXQStringListModel::data(const QModelIndex &index, int role) const
{
    switch (role)
    {
    case Qt::ForegroundRole:
        return m_rowColors.at(index.row()).color;
    case Roles::details:
        return m_rowColors.at(index.row()).details;
    default:
        return QStringListModel::data(index, role);
    }
}


bool AXQStringListModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    switch (role)
    {
    case Qt::ForegroundRole:
        m_rowColors[index.row()].color = value.value<QColor>();
        emit dataChanged(index, index);
        return true;
    case Roles::details:
        m_rowColors[index.row()].details = value.value<QString>();
//        emit dataChanged(index, index);
        return true;
    default:
        return QStringListModel::setData(index, value, role);
    }
}
