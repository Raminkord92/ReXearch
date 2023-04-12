#ifndef DLGPAPERDETAILS_H
#define DLGPAPERDETAILS_H

#include <QDialog>
#include <QGridLayout>
#include <QLabel>
#include <QLineEdit>
#include <QTextedit>
#include <Headers/Common.h>


namespace Ui {
class DlgPaperDetails;
}

class DlgPaperDetails : public QDialog
{
    Q_OBJECT

public:
    explicit DlgPaperDetails(const QStringList& paperDetail, QWidget *parent = nullptr);
    ~DlgPaperDetails();

private:
    Ui::DlgPaperDetails *ui;
    void initializeUI(const QStringList& paperDetail);
    QGridLayout* m_mainLayout;
    QLabel* m_lblAuthors;
    QLineEdit* m_lneAuthors;
    QLabel* m_lblTitle;
    QLineEdit* m_lneTitle;
    QLabel* m_lblSummary;
    QTextEdit* m_lneSummary;
    QLabel* m_lblLink;
    QLineEdit* m_lneLink;
    QLabel* m_lblDate;
    QLineEdit* m_lneDate;

};

#endif // DLGPAPERDETAILS_H
