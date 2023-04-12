#include <Headers/dlgpaperdetails.h>
#include "ui_dlgpaperdetails.h"



DlgPaperDetails::DlgPaperDetails( const QStringList &paperDetail, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DlgPaperDetails)
{
    ui->setupUi(this);
    this->setWindowTitle("Paper Detail");
    initializeUI(paperDetail);
}

DlgPaperDetails::~DlgPaperDetails()
{
    delete ui;
}

void DlgPaperDetails::initializeUI(const QStringList &paperDetail)
{
    m_mainLayout = new QGridLayout(this);
    m_lblAuthors  = new QLabel("Authors:", this);
    m_mainLayout->addWidget(m_lblAuthors, 0, 0);
    m_lneAuthors = new QLineEdit(this);
    m_lneAuthors->setReadOnly(true);
    m_lneAuthors->setText(paperDetail[(int)SearchTblColEnum::Authors]);
    m_mainLayout->addWidget(m_lneAuthors, 0, 1);

    m_lblTitle  = new QLabel("Title:", this);
    m_mainLayout->addWidget(m_lblTitle, 1, 0);
    m_lneTitle = new QLineEdit(this);
    m_lneTitle->setReadOnly(true);
    m_lneTitle->setText(paperDetail[(int)SearchTblColEnum::Title]);
    m_mainLayout->addWidget(m_lneTitle, 1, 1);

    m_lblSummary  = new QLabel("Summary:", this);
    m_mainLayout->addWidget(m_lblSummary, 2, 0);
    m_lneSummary = new QTextEdit(this);
    m_lneSummary->setReadOnly(true);
    m_lneSummary->setText(paperDetail[(int)SearchTblColEnum::Summary]);
    m_mainLayout->addWidget(m_lneSummary, 2, 1);

    m_lblDate  = new QLabel("Published:", this);
    m_mainLayout->addWidget(m_lblDate, 3, 0);
    m_lneDate = new QLineEdit(this);
    m_lneDate->setReadOnly(true);
    m_lneDate->setText(paperDetail[(int)SearchTblColEnum::Published]);
    m_mainLayout->addWidget(m_lneDate, 3, 1);

    m_lblLink  = new QLabel("Link:", this);
    m_mainLayout->addWidget(m_lblLink, 4, 0);
    m_lneLink = new QLineEdit(this);
    m_lneLink->setReadOnly(true);
    m_lneLink->setText(paperDetail[(int)SearchTblColEnum::Address]);
    m_mainLayout->addWidget(m_lneLink, 4, 1);
}
