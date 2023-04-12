#ifndef AXQSTRINGLISTMODEL_H
#define AXQSTRINGLISTMODEL_H

#include <QStringListModel>
#include <QObject>
#include <unordered_map>
#include <Headers/DBManager.h>

enum Roles
{
    details = Qt::UserRole + 1,
};

class AXQStringListModel : public QStringListModel
{
    Q_OBJECT
public:
    explicit AXQStringListModel(QObject *parent = nullptr);

    // QAbstractItemModel interface
public:
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::DisplayRole) override;

private:
    std::unordered_map<int, DashboardLabel> m_rowColors;
};

#endif // AXQSTRINGLISTMODEL_H
