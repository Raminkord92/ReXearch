#include <Headers/Models/axtablemodel.h>



AXTableModel::AXTableModel(QStringList modelHeaderData, QObject * parent) : QAbstractTableModel(parent)
{
    this->modelHeaderData = modelHeaderData;
}

int AXTableModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return modelData.size();
}

int AXTableModel::columnCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return modelHeaderData.size();
}

QVariant AXTableModel::data(const QModelIndex &index, int role) const
{
    if(!index.isValid() || index.row() >= modelData.size()
            || index.row() < 0 || index.column() >= modelHeaderData.size()
            || index.column() < 0)
    {
        return QVariant();
    }

    switch (role) {
    case Qt::DisplayRole:
        return modelData[index.row()][index.column()];
    case Qt::TextAlignmentRole:
        return Qt::AlignCenter;
    default:
        break;
    }

    return QVariant();
}

QVariant AXTableModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if(role != Qt::DisplayRole)
    {
        return QVariant();
    }

    if(orientation == Qt::Horizontal)
    {
        if(section >= columnCount() || section < 0)
        {
            return QVariant();
        }

        return modelHeaderData[section];
    }
    if (orientation == Qt::Vertical)
    {
        if (section >= rowCount() || section < 0)
        {
            return QVariant();
        }
        return section+1;
    }

    return QVariant();
}

Paper AXTableModel::getPaper(const QStringList &infos)
{
    Paper paper_t;
    QString authors_t = infos[(int)SearchTblColEnum::Authors];
    QStringList authorsList_t = authors_t.split(",");
    for (auto&& author_t : authorsList_t)
    {
        Author authorObj_t;
        authorObj_t.name = author_t;
        paper_t.authors.append(authorObj_t);
    }
    paper_t.address = infos[(int)SearchTblColEnum::Address];
    paper_t.published = infos[(int)SearchTblColEnum::Published];
    paper_t.summary = infos[(int)SearchTblColEnum::Summary];
    paper_t.title = infos[(int)SearchTblColEnum::Title];
    return paper_t;
}

bool AXTableModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if(index.isValid() && role == Qt::EditRole)
    {
        QString stringValue = value.toString();

        // make sure the new entry name is at least one character
        if(stringValue.size() == 0)
        {
            stringValue = "";
        }

        modelData[index.row()][index.column()] = stringValue;
        emit dataChanged(index, index);
        return true;
    }
    return false;
}

bool AXTableModel::insertRows(int position, int rows, const QModelIndex &index)
{
    Q_UNUSED(index);
    beginInsertRows(QModelIndex(), position, position + rows - 1);

    for (int row = 0; row < rows; row++)
    {
        QStringList rowItem;
        for(int column = 0; column < columnCount(); column++)
        {
            rowItem.append(QString(" "));
        }

        modelData.insert(position, rowItem);
    }

    endInsertRows();

    return true;
}

bool AXTableModel::removeRows(int position, int rows, const QModelIndex &index)
{
    Q_UNUSED(index);
    beginRemoveRows(QModelIndex(), position, position + rows - 1);

    for(int row = position; row <= position + rows - 1; ++row)
    {
        modelData.removeFirst();
    }
    qDebug() << "SIZE OF LIST" << modelData.isEmpty() << modelData.size();
    endRemoveRows();

    return true;
}

bool AXTableModel::swapRows(int source, int destination)
{
    if(!isValidRow(source) || !isValidRow(destination))
    {
        return false;
    }

    if(source == destination)
    {
        return true;
    }

    QStringList sourceValues = rowValues(source);
    QStringList destinationValues = rowValues(destination);

    setRowValues(destination, sourceValues);
    emit dataChanged(index(destination, 0), index(destination, columnCount() - 1));

    setRowValues(source, destinationValues);
    emit dataChanged(index(source, 0), index(source, columnCount() - 1));

    return true;
}

bool AXTableModel::isValidRow(int row)
{
    return row >= 0 && row < rowCount();
}

bool AXTableModel::isValidColumn(int column)
{
    return column >= 0 && column < columnCount();
}

QStringList AXTableModel::rowValues(int row)
{
    if(isValidRow(row))
    {
        return modelData[row];
    }
    return QStringList();
}

bool AXTableModel::setRowValues(int row, QStringList values)
{
    if(isValidRow(row))
    {
        modelData[row] = values;
        emit rowCountChanged();
        return true;
    }

    return false;
}

Qt::ItemFlags AXTableModel::flags(const QModelIndex &index) const
{
    Q_UNUSED(index);
    return Qt::ItemIsEnabled | Qt::ItemIsSelectable ;
}

void AXTableModel::clearAll()
{
    int currentRowCount = rowCount();
    if(currentRowCount > 0)
    {
        removeRows(0, currentRowCount);
    }
    emit rowCountChanged();
}

QStringList AXTableModel::paperStructToStringList(const Paper &paper)
{
    QStringList output_t;
    QString authorNames;
    for (auto&& author : paper.authors)
    {
        if (author.name == paper.authors.last().name)
        {
            authorNames += author.name;
        }
        authorNames += (author.name + ", ");
    }
    output_t << authorNames;
    output_t << paper.title;
    output_t << paper.summary;
    output_t << paper.published;
    output_t << paper.address;
    return output_t;
}


