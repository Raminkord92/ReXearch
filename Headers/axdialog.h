#ifndef AXDIALOG_H
#define AXDIALOG_H

#include "Headers/Common.h"
#include "Headers/DBManager.h"
#include "qabstractbutton.h"
#include "qradiobutton.h"
#include <QDialog>


namespace Ui {
class AXDialog;
}

QString getRadioButtonStyleSheet(const QColor& color);
QString getLableLineEditStyleSheet(const QColor& color);


class AXDialog : public QDialog
{
    Q_OBJECT

public:
    ~AXDialog();

    QColor getSelectedColor() const;
    void setSelectedColor(const QColor &newSelectedColor);
    explicit AXDialog(QWidget *parent = nullptr, QString titleName = "");
    QString getLblName() const;
    QString getDetails() const;
    void setDeails(const QString &newDetails);
    void initailizeDialog(const DashboardLabel& labelInfo);
    DashboardLabel getOutput() const;
    void setName(const QString &newName);

    DashboardLabel getOldLabel() const;
    void setOldLabel(const DashboardLabel &newOldLabel);

    QColor getDefaultColor() const;

private slots:
    void on_btnCusomColor_clicked();
    void on_btnOK_clicked();
    void on_btnCancel_clicked();

    void on_lneLblName_textChanged(const QString &arg1);

signals:
    void emitChangedLabel(QString name, DashboardLabel label, int flag);
private:
    Ui::AXDialog *ui;
    void createCusomColorUI();
    void initialCheckAndConfig();
    QColor m_selectedColor;
    QRadioButton* m_customColor;
    QString m_deails;
    QString m_name;
    DashboardLabel oldLabel;
    const QColor defaultColor = Qt::green;

};

#endif // AXDIALOG_H
