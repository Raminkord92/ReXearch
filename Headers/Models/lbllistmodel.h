#ifndef LBLLISTMODEL_H
#define LBLLISTMODEL_H

#include <QAbstractListModel>
#include <QObject>
#include <label.h>

class LblListModel : public QAbstractListModel
{
    Q_OBJECT
public:
    LblListModel(QObject* parent = nullptr);
    int rowCount(const QModelIndex &parent) const override;
    QVariant data(const QModelIndex &index, int role) const override;
    bool insertRows(int row, int count, const QModelIndex &parent) override;
    void populate(QList<Label> labels);

private:
    QList<Label> m_Labels;

};

#endif // LBLLISTMODEL_H
