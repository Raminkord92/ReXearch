#include "Headers/DBManager.h"

DBManager::DBManager(QObject *parent)
    : QObject{parent}
{
    m_rexearchDB = QSqlDatabase::addDatabase(dbType);
    m_rexearchDB.setDatabaseName(dbName);
    if (!m_rexearchDB.open())
    {
        qFatal("Failed to open the database");
        return;
    }
    createTables();
}

QVector<DashboardLabel> DBManager::getLabels() const
{
    QSqlQuery query;
    query.prepare("SELECT * from " + tblLabelName);
    QVector<DashboardLabel> output_v1;
    if (!query.exec())
    {
        qDebug() << "error in red labels from table in database " << query.lastError();
        return output_v1;
    }

    while (query.next())
    {
        QSqlRecord rec = query.record();
        auto nameIndex = rec.indexOf(tblLabelColNames[DashboardLabelCol::name]);
        QString name = query.value(nameIndex).toString();
        auto detailsIndex = rec.indexOf(tblLabelColNames[DashboardLabelCol::details]);
        QString details = query.value(detailsIndex).toString();
        auto colorIndex = rec.indexOf(tblLabelColNames[DashboardLabelCol::color]);
        QString color = query.value(colorIndex).toString();
        output_v1.emplaceBack(name, details, color);
    }

    return output_v1;
}

bool DBManager::addLblToDB(const DashboardLabel &lbl)
{
    QSqlQuery query;
    QString name = tblLabelColNames[DashboardLabelCol::name];
    QString details = tblLabelColNames[DashboardLabelCol::details];
    QString color = tblLabelColNames[DashboardLabelCol::color];
    QString nameBinded = ":name";
    QString detailsBinded = ":details";
    QString colorBinded = ":color";
    QString queryString = QString("INSERT INTO %1 (%2, %3, %4) VALUES (%5, %6, %7)").arg(tblLabelName, name, details, color, nameBinded, detailsBinded, colorBinded);
    qDebug() << "Insert query is " << queryString;
    query.prepare(queryString);
    query.bindValue(nameBinded, lbl.name);
    query.bindValue(detailsBinded, lbl.details);
    query.bindValue(colorBinded, lbl.color);
    if (!query.exec())
    {
        qDebug() << "addLblToDB This record cannot be added";
        return false;
    }
    return true;
}

bool DBManager::removeFromDB(const QString &name)
{
    QSqlQuery query;
    QString queryString = QString("DELETE FROM %1 WHERE name = '%2'").arg(tblLabelName, name);
    qDebug() << "removeFromDB" << queryString;
    query.prepare(queryString);
    if (!query.exec())
    {
        qDebug() << "removeFromDB" << query.lastError();
        return false;
    }

    emit emitLblTblUpdated();
    return true;
}

bool DBManager::addLblToDB(const QString &name, const QString &details, const QString &color)
{
    return addLblToDB(DashboardLabel(name, details, color));
}

void DBManager::updateLabel(const QString &name, const DashboardLabel &label, int flag)
{
    qDebug() << "SALAM IN updateLabel";
    QString name_ = tblLabelColNames[DashboardLabelCol::name];
    QString details_ = tblLabelColNames[DashboardLabelCol::details];
    QString color_ = tblLabelColNames[DashboardLabelCol::color];
    bool colorChanged = false;
    QString stringQuery = QString("UPDATE %1 SET").arg(tblLabelName);
    if (flag | CHANGED_LABEL_COLOR)
    {
        stringQuery += QString(" %1 = '%2',").arg(color_, label.color.name());
        colorChanged = true;
    }
    bool nameChanged = false;
    if (flag | CHANGED_LABEL_NAME)
    {
        stringQuery += QString("%1 = '%2',").arg(name_, label.name);
        nameChanged = true;
    }
    bool detailChanged = false;
    if (flag | CHANGED_LABEL_DETAIL)
    {
        stringQuery += QString("%1 = '%2',").arg(details_, label.details);
        detailChanged = true;
    }
    stringQuery.remove(stringQuery.length()-1, 1);
    stringQuery += QString(" WHERE %1 = '%2'").arg(name_, name);

    QSqlQuery query;
    qDebug() << "QUERY FOR UPSATE" << stringQuery;
    query.prepare(stringQuery);
    if (!query.exec())
    {
        qDebug() << "Can't update the table error: " << query.lastError();
        return;
    }
    emit emitLblTblUpdated();
}

bool DBManager::createTables()
{
    QSqlQuery query;
    QString name = tblLabelColNames[DashboardLabelCol::name];
    QString details = tblLabelColNames[DashboardLabelCol::details];
    QString color = tblLabelColNames[DashboardLabelCol::color];
    QString queryCreate = QString("CREATE TABLE IF NOT EXISTS  %1 ( "
                                  "id INTEGER PRIMARY KEY AUTOINCREMENT,"
                                  "%2 TEXT UNIQUE,"
                                  "%3 TEXT,"
                                  "%4 TEXT)").arg(tblLabelName, name, details, color);
    qDebug() << "Create table query" << queryCreate;
    query.prepare(queryCreate);
    if (!query.exec())
    {
        qDebug() << "EROR " << query.lastError();
        qFatal("Failed to create labels table");
        return false;
    }

    addLblToDB("Home", "",  "black");
    return true;
}
