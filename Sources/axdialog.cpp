#include "Headers/axdialog.h"
#include "ui_axdialog.h"

#include <QButtonGroup>
#include <QMessageBox>
#include <QColorDialog>

AXDialog::AXDialog(QWidget *parent, QString titleName) :
    QDialog(parent),
    ui(new Ui::AXDialog)
{
    ui->setupUi(this);

    this->setWindowTitle(titleName);
    QIcon icon = windowIcon();
    Qt::WindowFlags flags = windowFlags();
    Qt::WindowFlags helpFlag = Qt::WindowContextHelpButtonHint;
    flags = flags & (~helpFlag);
    setWindowFlags(flags);
    setWindowIcon(icon);
    createCusomColorUI();
    initialCheckAndConfig();
    connect(this, &AXDialog::emitChangedLabel, &DBManager::getInstance(), &DBManager::updateLabel);
}

QString AXDialog::getLblName() const
{
    return m_name;
}

AXDialog::~AXDialog()
{
    delete ui;
}

void AXDialog::on_btnCusomColor_clicked()
{
    QColorDialog dialog(this);
    dialog.setOption(QColorDialog::ShowAlphaChannel, true);
    //default color for qcolor dialog
    dialog.setCurrentColor (defaultColor);
    if (dialog.exec())
    {
        QColor color = dialog.selectedColor();
        setSelectedColor(color);
        m_customColor->setStyleSheet(getRadioButtonStyleSheet(getSelectedColor()));
    }
}

QColor AXDialog::getSelectedColor() const
{
    return m_selectedColor;
}

void AXDialog::setSelectedColor(const QColor &newSelectedColor)
{
    m_selectedColor = newSelectedColor;
    m_customColor->setStyleSheet(getRadioButtonStyleSheet(newSelectedColor));
}

QString getRadioButtonStyleSheet(const QColor& color)
{
  // Define the style sheet string
  QString style_sheet =
    "QRadioButton::indicator {"
      "background-color: %1;"
      "border: 2px solid %1;"
    "}";

  // Replace the %1 placeholders with the color value
  style_sheet = style_sheet.arg(color.name());

  return style_sheet;
}


void AXDialog::createCusomColorUI()
{
    m_customColor = new QRadioButton("");
    setSelectedColor(defaultColor);
    m_customColor->setStyleSheet(getRadioButtonStyleSheet(defaultColor));
    ui->customColorLayoutH->insertWidget(1, m_customColor);
}

void AXDialog::initialCheckAndConfig()
{
    ui->textEditQuery->setDisabled(true);
    if (ui->lneLblName->text().isEmpty())
        ui->btnOK->setDisabled(true);
}

QColor AXDialog::getDefaultColor() const
{
    return defaultColor;
}

DashboardLabel AXDialog::getOldLabel() const
{
    return oldLabel;
}

void AXDialog::setOldLabel(const DashboardLabel &newOldLabel)
{
    oldLabel = newOldLabel;
}

void AXDialog::initailizeDialog(const DashboardLabel &paper)
{
    setName(paper.name);
    setDeails(paper.details);
    setSelectedColor(paper.color);
    ui->lneLblName->setText(paper.name);
    setOldLabel(paper);
}

DashboardLabel AXDialog::getOutput() const
{
    return DashboardLabel(getLblName(), getDetails(), getSelectedColor().name());
}

void AXDialog::setName(const QString &newName)
{
    m_name = newName;
}

QString AXDialog::getDetails() const
{
    return m_deails;
}

void AXDialog::setDeails(const QString &newDetails)
{
    m_deails = newDetails;
    ui->textEditQuery->setText(newDetails);
}


void AXDialog::on_btnOK_clicked()
{
    qDebug() << "on_btnOK_clicked";
    auto name = ui->lneLblName->text();
    setName(name);
    //cheeck the name if it already exists
    auto labels = DBManager::getInstance().getLabels();
    int numTimes = 0;
    auto oldLabel = getOldLabel();

    for (int i = 0; i < labels.count(); i++)
    {
        if (labels[i].name == name && oldLabel.name != name)
            numTimes++;
        if (numTimes > 0)
        {
            qDebug() << "on_btnOK_clicked";
            QMessageBox::critical(this, "Error", "Label name should be unique. This label name already exists.");
            return;
        }
    }

    int changedFlag = 0;
    if (oldLabel.name != name)
    {
        changedFlag |= CHANGED_LABEL_NAME;
    }

    if (oldLabel.color != getSelectedColor().name())
    {
        changedFlag |= CHANGED_LABEL_COLOR;
    }
    if (oldLabel.details != getDetails().trimmed())
    {
        changedFlag |= CHANGED_LABEL_DETAIL;
    }

    if (changedFlag == 0)
        accept();


    emit emitChangedLabel(oldLabel.name, DashboardLabel(name, getDetails(), getSelectedColor().name()), changedFlag);


    accept();
}


void AXDialog::on_btnCancel_clicked()
{
    reject();
}


void AXDialog::on_lneLblName_textChanged(const QString &arg1)
{
    arg1.isEmpty() ? ui->btnOK->setDisabled(true) : ui->btnOK->setEnabled(true);

}

