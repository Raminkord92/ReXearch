#include "Headers/mainwindow.h"
#include "ui_mainwindow.h"

#include <Headers/AXDialog.h>
#include <QDomDocument>
#include <QFile>
#include <Headers/Common.h>
#include <QXmlStreamReader>
#include <ThirdParties/Libs/pugixml/headers/pugixml.hpp>
#include <QMenu>
#include <QMessageBox>
#include <QPainter>
#include <QSplitter>
#include <Headers/dlgpaperdetails.h>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    //Main Page
    manager = new QNetworkAccessManager(this);
    //    ui->headerLbl->installEventFilter(this);
    ui->searchResultView_->installEventFilter(this);
    setWindowStyle();
    setMainPageTblModel();
    setClickableSignalDelegate();
    setSingleSelectionViewStyle();
    ui->searchResultView_->setContextMenuPolicy(Qt::CustomContextMenu);
    ui->lblListView_->setContextMenuPolicy(Qt::CustomContextMenu);
    ui->lineEditSearch_->setPlaceholderText("Search...");
    setDashboardPageListModel();
    initializeSearchComboBox();
    ui->topHLayoutMainPage->setObjectName("headerWidget");
    ui->topHLayoutMainPage->setStyleSheet("#headerWidget {border-image: url(://resources/header.jpg) 0 0 0 0 stretch stretch}");
    m_searchResultTblModel->rowCount() == 0 ? ui->plusBtn_->setDisabled(true) : ui->plusBtn_->setEnabled(true);
    ui->splitterMainViews->setStretchFactor(0, 1);
    ui->splitterMainViews->setStretchFactor(1, 5);
    ui->rmAllBtn_->setHidden(true);
    initializeDashboard();
    initializeConnections();
}

void MainWindow::initializeDashboard()
{
    m_searchResultTblModel->clearAll();
    disconnect(ui->lblListView_->selectionModel(), &QItemSelectionModel::currentRowChanged, this, &MainWindow::currentRowLblChanged);
    auto dashboardLabels = DBManager::getInstance().getLabels();
    for (auto&& dashboadLabel : dashboardLabels)
        appendLabel(dashboadLabel);

    m_homeIndex = ui->lblListView_->model()->index(0, 0);
    ui->lblListView_->setCurrentIndex(m_homeIndex);

    connect(ui->lblListView_->selectionModel(), &QItemSelectionModel::currentRowChanged, this, &MainWindow::currentRowLblChanged);
}

void MainWindow::initializeConnections()
{
    //    connect(ui->searchResultView_, &QTableView::customContextMenuRequested, this, &MainWindow::slotCustomContextMenumTblView);
    connect(ui->lblListView_, &AXListView::contextMenuRequested, this, &MainWindow::slotCustomContextMenumListView);
    connect(ui->cmbSearchItems_, &QComboBox::currentIndexChanged, this, &MainWindow::on_cmbSearchItems_currentIndexChanged);
    connect(ui->searchBtn_, &QPushButton::clicked, this, &MainWindow::on_searchBtn_clicked);
    connect(ui->rmAllBtn_, &QPushButton::clicked, this, &MainWindow::on_rmAllBtn_clicked);
    connect(ui->lineEditSearch_, &QLineEdit::returnPressed, this, &MainWindow::on_lineEditSearch_returnPressed);
    connect(m_searchResultTblModel, &AXTableModel::rowCountChanged, this, [this](){
        m_searchResultTblModel->rowCount() == 0 || ui->lblListView_->currentIndex() != m_homeIndex? ui->plusBtn_->setDisabled(true) : ui->plusBtn_->setEnabled(true);
    });
    connect(&DBManager::getInstance(), &DBManager::emitLblTblUpdated, this, &MainWindow::updateDashboardLbl);
    connect(ui->lblListView_, &AXListView::dblClicked, this, &MainWindow::on_lblListView_doubleClicked);
    connect(ui->searchResultView_, &AXTableView::emitDoubleClicked, this, &MainWindow::on_searchResultView__doubleClicked);
}

void MainWindow::setWindowStyle()
{
    this->setWindowTitle("ReXearch");
}

void MainWindow::setMainPageTblModel()
{
    QStringList modelHeaderData = {
        SearchTblHeaders.at(SearchTblColEnum::Authors),
        SearchTblHeaders.at(SearchTblColEnum::Title),
        SearchTblHeaders.at(SearchTblColEnum::Summary),
        SearchTblHeaders.at(SearchTblColEnum::Published),
        SearchTblHeaders.at(SearchTblColEnum::Address)};
    m_searchResultTblModel = new AXTableModel(modelHeaderData, this);
    QSortFilterProxyModel *proxyModel = new QSortFilterProxyModel(this);
    proxyModel->setFilterCaseSensitivity(Qt::CaseInsensitive);

    proxyModel->setSourceModel(m_searchResultTblModel);
    ui->searchResultView_->setModel(proxyModel);

    connect(ui->lneFilter, &QLineEdit::textChanged, proxyModel, &QSortFilterProxyModel::setFilterFixedString);
}

void MainWindow::setSingleSelectionViewStyle()
{
    ui->searchResultView_->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->searchResultView_->setSelectionBehavior(QAbstractItemView::SelectRows);
}

void MainWindow::setClickableSignalDelegate()
{
    clickableDelegate = new ClickableLinkDelegate(this);
    ui->searchResultView_->setItemDelegateForColumn(4, clickableDelegate);
    connect(clickableDelegate, &ClickableLinkDelegate::clicked, this, &MainWindow::openLink);
}

void MainWindow::setDashboardPageListModel()
{
    m_lblModel = new AXQStringListModel(this);
    // set the font size and family
    ui->lblListView_->setModel(m_lblModel);
    ui->lblListView_->setDragDropMode(QAbstractItemView::NoDragDrop);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::get(QString location)
{
    qInfo() << "Getting from server...";
    QNetworkReply* reply = manager->get(QNetworkRequest(QUrl(location)));
    connect(reply,&QNetworkReply::readyRead,this,&MainWindow::readyRead);
}

void MainWindow::post(QString location, QByteArray data)
{
    qInfo() << "Posting to the server...";
    QNetworkRequest request = QNetworkRequest(QUrl(location));
    request.setHeader(QNetworkRequest::ContentTypeHeader, "test/plain"); //mime type

    QNetworkReply* reply = manager->post(request,data);
    connect(reply,&QNetworkReply::readyRead,this,&MainWindow::readyRead);
}

void MainWindow::updateDashboardLbl()
{
    m_lblModel->setStringList(QStringList{});
    initializeDashboard();
    ui->lblListView_->setCurrentIndex(m_homeIndex);
}

void MainWindow::readyRead()
{
    QNetworkReply* reply = qobject_cast<QNetworkReply*>(sender());
    QEventLoop loop;
    connect(reply, &QNetworkReply::finished, &loop, &QEventLoop::quit);
    connect(&loop, &QEventLoop::destroyed, this, &MainWindow::arxivXmlReader);
    loop.exec();
    data = reply->readAll();
}


bool MainWindow::eventFilter(QObject *monitoredObj, QEvent *event)
{
    hanleSizeEventChng(monitoredObj, event);
    return false;
}


void MainWindow::hanleSizeEventChng(QObject *monitoredObj, QEvent *event)
{
    if (event->type() == QEvent::Resize)
    {

        if (monitoredObj == ui->searchResultView_)
        {
            searchViewColumnSizeHandler();
        }

        //        if (monitoredObj == ui->headerLbl)
        //        {
        //            IconHandler(ui->headerLbl, "://resources/header.jpg");
        //        }

    }
}

void MainWindow::IconHandler(QLabel *monitoredObj, QString pixMapAddr)
{
    QPixmap p(pixMapAddr); // load pixmap
    // get label dimensions
    int w = monitoredObj->width();
    int h = monitoredObj->height();
    // set a scaled pixmap to a w x h window keeping its aspect ratio
    monitoredObj->setPixmap(p.scaled(w,h));
}

void MainWindow::searchViewColumnSizeHandler()
{
    int w = ui->searchResultView_->width();
    ui->searchResultView_->setColumnWidth(0, 0.20*w);
    ui->searchResultView_->setColumnWidth(1, 0.3*w);
    ui->searchResultView_->setColumnWidth(2, 0.3*w);
    ui->searchResultView_->setColumnWidth(3, 0.1*w);
    ui->searchResultView_->setColumnWidth(4, 0.1*w);
    ui->searchResultView_->setColumnWidth(5, 0.1*w);
}


QString MainWindow::generatedQuery() const
{
    return m_generatedQuery;
}

void MainWindow::appendLabel(const DashboardLabel &lbl)
{
    int lastRow = m_lblModel->rowCount();
    m_lblModel->insertRow(lastRow);
    QModelIndex lastIndex = ui->lblListView_->model()->index(lastRow,0);
    m_lblModel->setData(lastIndex, QColor(lbl.color), Qt::ForegroundRole);
    m_lblModel->setData(lastIndex, lbl.details, Roles::details);
    m_lblModel->setData(lastIndex, lbl.name, Qt::DisplayRole);
    ui->lblListView_->setCurrentIndex(lastIndex);
}


void MainWindow::arxivXmlReader()
{
    if (data.size() == 0)
        return;
    pugi::xml_document doc;
    pugi::xml_parse_result result = doc.load_string(data);
    if (result.status != pugi::xml_parse_status::status_ok)
    {
        std::cout << "can't load the xml" << result.description();
        return;
    }
    else
    {
        std::cout << "Successful in reading the XML" << std::endl;
    }
    m_searchResultTblModel->clearAll();

    for (pugi::xml_node entry = doc.child("feed").child("entry"); entry; entry = entry.next_sibling("entry"))
    {
        Paper paper_t;
        paper_t.summary =  entry.child_value("summary");
        paper_t.title = entry.child_value("title");
        paper_t.published = entry.child_value("published");
        paper_t.address = entry.child_value("id");

        for (pugi::xml_node author = entry.child("author"); author; author = author.next_sibling("author"))
        {
            Author author_t;
            author_t.name = author.child_value("name");
            paper_t.authors.append(author_t);
        }
        m_searchResultTblModel->appendRow(paper_t);
    }

}

void MainWindow::openLink(QString text)
{
    QDesktopServices::openUrl(QUrl(text));
}

void MainWindow::search(QString query, SearchQueryType type)
{
    if (m_timer.elapsed() > 3000)
    {
        qDebug() << "SALAM SEARCH";
        if (!IsInternetAvailable())
        {
            qDebug() << "NO INTERNET";

            QMessageBox::warning(this, "Warning", "You are not connected to the internet.");
            return;
        }
        qDebug() << "SEARCH " << query;
        if (query.isEmpty())
            QMessageBox::warning(this, "Warning", "Please write something to search.");

        m_searchResultTblModel->clearAll();

        if (type == SearchQueryType::NEW)
        {
            ArxiveQueryGenerator searchGen(query, m_searchField, ArxiveQueryGenerator::SearchType::Simple);
            searchGen.ObtainQuery();
            m_generatedQuery = searchGen.getGeneratedQuery();
            qDebug() << "Query" << m_generatedQuery;
            m_timer.start();

        }
        else if (type == SearchQueryType::DB)
        {
            m_generatedQuery = query;
        }

        get(m_generatedQuery);
    }
    else
    {
        ui->searchBtn_->setDisabled(true);
        qDebug() << "WAIT 3 seconds";
        // Start a timer to re-enable the button after 3 seconds
        QTimer::singleShot(3000 - m_timer.elapsed(), [this](){
            ui->searchBtn_->setEnabled(true);
        });

    }


}

void MainWindow::on_searchBtn_clicked()
{
    ui->lblListView_->setCurrentIndex(m_homeIndex);

    search(ui->lineEditSearch_->text());

}

void MainWindow::slotCustomContextMenumListView(const QPoint &point)
{
    qDebug() << "SALAM slotCustomContextMenumListView";
    if (m_lblModel->rowCount() < 1 || ui->lblListView_->indexAt(point) == m_homeIndex)
        return;
    QMenu* menu = new QMenu(this);
    QAction* editActionCell = new QAction("Edit");
    QAction* rmActionCell = new QAction("Remove");

    connect(editActionCell, &QAction::triggered, this, &MainWindow::slotEditLabel);
    connect(rmActionCell, &QAction::triggered, this, &MainWindow::slotRmLabel);
    menu->addAction(editActionCell);
    menu->addAction(rmActionCell);
    menu->popup(ui->lblListView_->viewport()->mapToGlobal(point));

}

void MainWindow::slotEditLabel()
{
    auto hoveredIndex = ui->lblListView_->hoveredIndex();
    qDebug() << "SlotEditLabel " << hoveredIndex;
    if (hoveredIndex == m_homeIndex)
        return;
    on_lblListView_doubleClicked(hoveredIndex);
}

void MainWindow::slotRmLabel()
{
    auto hoveredIndex = ui->lblListView_->hoveredIndex();
    qDebug() << "SlotEditLabel " << hoveredIndex;
    if (hoveredIndex == m_homeIndex)
        return;
    QString name = ui->lblListView_->model()->data(hoveredIndex, Qt::DisplayRole).toString();
    DBManager::getInstance().removeFromDB(name);
}

//void MainWindow::slotCustomContextMenumTblView(const QPoint pos)
//{
//    QMenu* menu = new QMenu(this);
//    QAction* saveAsActionCell = new QAction("Save This Cell");
//    QAction* saveAsActionRow = new QAction("Save This Row");
//    saveAsActionCell->setIcon(QIcon(":/resources/SaveAsIcon.png"));
//    saveAsActionRow->setIcon(QIcon(":/resources/SaveAsIcon.png"));
//    connect(saveAsActionCell, &QAction::triggered, this, &MainWindow::saveAsActionSlotCell);
//    connect(saveAsActionRow, &QAction::triggered, this, &MainWindow::saveAsActionSlotRow);
//    menu->addAction(saveAsActionCell);
//    menu->addAction(saveAsActionRow);
//    if (m_searchResultTblModel->rowCount() != 0)
//    {
//        menu->popup(ui->searchResultView_->viewport()->mapToGlobal(pos));
//    }
//}

void MainWindow::saveAsActionSlotCell()
{

}

void MainWindow::saveAsActionSlotRow()
{
    QModelIndexList selection = ui->searchResultView_->selectionModel()->selectedRows();
    //the table is single selected. In case of when this behavior was changed
    // we should use loop
    if (selection.count() != 1)
        return;

    for (int i = 0; i < selection.count(); i++)
    {
        QModelIndex index = selection.at(i);
        int row_ = index.row();
        int col_ = index.column();
        QModelIndex index1 =  ui->searchResultView_->model()->index(row_, 0, QModelIndex());
        QString text = ui->searchResultView_->model()->data(index1).toString();
        qDebug() << "(" << row_ << ", " << col_ << "): " <<  "Selected row " << text;
    }

    //windows desktop address
    auto desktopddress = QDir::homePath() + QDir::separator() + "Desktop";
    QString fileName = QFileDialog::getSaveFileName(this, tr("Save File"), desktopddress, tr("Text Files (*.txt)"));

}

void MainWindow::on_actionExit_triggered()
{
    QApplication::quit();
}

void MainWindow::on_lineEditSearch_returnPressed()
{
    qDebug() << "HI";
    on_searchBtn_clicked();
}


void MainWindow::on_rmAllBtn_clicked()
{
    m_searchResultTblModel->clearAll();
    ui->lineEditSearch_->clear();
}


void MainWindow::initializeSearchComboBox()
{
    ui->cmbSearchItems_->addItems(SearchFieldsText);
}

void MainWindow::on_cmbSearchItems_currentIndexChanged(int index)
{
    Q_UNUSED(index);
    QString currentSearchFieldText = ui->cmbSearchItems_->currentText();
    if (SearchFieldsMap.find(currentSearchFieldText) != SearchFieldsMap.end())
    {
        m_searchField = SearchFieldsMap.at(currentSearchFieldText);
        return;
    }
}

void MainWindow::on_searchResultView__doubleClicked(const QModelIndex &index)
{
    QStringList input_t;

    for (int i = 0; i < (int)SearchTblColEnum::Max; i++)
    {
        const QModelIndex& index_t = ui->searchResultView_->model()->index(index.row(), i);
        input_t << ui->searchResultView_->model()->data(index_t).toString();
    }

    DlgPaperDetails dlgDetails(input_t);
    dlgDetails.exec();
}

void MainWindow::on_plusBtn__clicked()
{
    AXDialog* lblDlg = new AXDialog(this, "Choose Label");
    lblDlg->setDeails(generatedQuery());

    if (lblDlg->exec() != QDialog::Accepted)
        return;

    DashboardLabel lbl(lblDlg->getOutput());
    if (!DBManager::getInstance().addLblToDB(lbl))
        return;
    appendLabel(lbl);
}

void MainWindow::on_lblListView_doubleClicked(const QModelIndex &index)
{
    QString name = index.model()->data(index, Qt::DisplayRole).toString();
    QString color = index.model()->data(index, Qt::ForegroundRole).toString();
    QString details = index.model()->data(index, Roles::details).toString();
    AXDialog* lblDlg = new AXDialog(this, "Edit label");
    lblDlg->initailizeDialog(DashboardLabel(name, details, color));
    if (lblDlg->exec() != QDialog::Accepted)
        return;
}

void MainWindow::currentRowLblChanged(const QModelIndex &currentIndex, const QModelIndex &prevIndex)
{
    Q_UNUSED(prevIndex);

    if (!currentIndex.isValid())
    {
        return;
    }
    if (currentIndex == m_homeIndex)
    {
        m_searchResultTblModel->clearAll();
        return;
    }
    auto model = ui->lblListView_->model();
    this->search(model->data(currentIndex, Roles::details).toString(), SearchQueryType::DB);
}


bool IsInternetAvailable()
{

#ifdef _WIN32
    if (!InternetCheckConnection (L"https://arxiv.org",FLAG_ICC_FORCE_CONNECTION,0))
    {
        return false;
    }
    return true;
#else
    struct hostent *he;
    struct in_addr **addr_list;
    int i;

    if ((he = gethostbyname("https://arxiv.org")) == NULL) {
        return false;
    }

    addr_list = (struct in_addr **) he->h_addr_list;

    for (i = 0; addr_list[i] != NULL; i++)
    {
        if (inet_ntoa(*addr_list[i]) != NULL)
        {
            return true;
        }
    }

    return false;

#endif
}

