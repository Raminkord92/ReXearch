#include "lbllistmodel.h"

LblListModel::LblListModel(QObject* parent): QAbstractListModel{parent}
{

}

int LblListModel::rowCount(const QModelIndex &parent) const
{
    return m_Labels.size();
}

QVariant LblListModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();
    if (role == Qt::DisplayRole)
    {
        if (index.column() == 0)
        {
            return m_Labels[index.row()].lblName();
        }
    }
    return QVariant();
}

bool LblListModel::insertRows(int row, int count, const QModelIndex &parent)
{

}
