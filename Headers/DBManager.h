#ifndef DBMANAGER_H
#define DBMANAGER_H

#include <QSqlDatabase>
#include <QString>
#include <QList>
#include <QStringList>
#include <QDebug>
#include <QSqlQuery>
#include <QMap>
#include <QSqlRecord>
#include <QtSql>
#include <QObject>
#include <QVector>

#define CHANGED_LABEL_NAME 0x01
#define CHANGED_LABEL_COLOR 0x02
#define CHANGED_LABEL_DETAIL 0x04

struct DashboardLabel
{
    QString name;
    QString details;
    QColor color;
    DashboardLabel() = default;
    DashboardLabel(QString name, QString details, QString color)
    {
        this->name = name;
        this->details = details;
        this->color = color;
    }
};

enum class DashboardLabelCol
{
    name,
    details,
    color
};

using LblTableMap = QMap<DashboardLabelCol, QString>;
class DBManager : public QObject
{
    Q_OBJECT
public:
    QVector<DashboardLabel> getLabels() const;
    bool addLblToDB(const DashboardLabel& lbl);
    bool removeFromDB(const QString& name);
    bool addLblToDB(const QString& name, const QString& details, const QString& color);
    static DBManager& getInstance()
    {
        static DBManager instance;
        return instance;
    }

public slots:
    void updateLabel(const QString& name, const DashboardLabel& label, int flag);

 signals:
    void emitLblTblUpdated();
private:
    explicit DBManager(QObject *parent = nullptr);
    ~DBManager()
    {
        m_rexearchDB.close();
    }

    DBManager(const DBManager&) = delete;
    DBManager& operator=(const DBManager&) = delete;
    DBManager(const DBManager&&) = delete;
    DBManager& operator=(const DBManager&&) = delete;

    bool createTables();
    QVector<DashboardLabel> m_labels;
    QSqlDatabase m_rexearchDB;
    const QString tblLabelName = "labels";
    const LblTableMap tblLabelColNames = {{DashboardLabelCol::name, "name"},
                                       {DashboardLabelCol::details, "details"},
                                       {DashboardLabelCol::color, "color"}};
    const QString dbName = "./rexearch.db";
    const QString dbType = "QSQLITE";
};

#endif // DBMANAGER_H
