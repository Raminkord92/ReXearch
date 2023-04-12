#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#ifdef _WIN32
#include <Windows.h>
#include <wininet.h>
#pragma comment(lib, "Wininet.lib")

#else
#include <netdb.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <errno.h>
#endif

#include <QMainWindow>
#include <QNetworkAccessManager>
#include <QDebug>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QAuthenticator>
#include <QNetworkProxy>
#include <QDomNode>
#include <memory>
#include <iostream>
#include <QLabel>
#include <Headers/Models/axtablemodel.h>
#include <QDesktopServices>
#include "Headers/Delegates/clickablelinkdelegate.h"
#include <QSizePolicy>
#include <QStringListModel>
#include <Headers/Models/axqstringlistmodel.h>
#include <QFileDialog>
#include <Headers/arxivequerygenerator.h>
#include <QElapsedTimer>
#include <Headers/DBManager.h>


QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

bool IsInternetAvailable();


class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    enum class SearchQueryType
    {
        DB,
        NEW
    };
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    QString generatedQuery() const;
    void appendLabel(const DashboardLabel& lbl);
public slots:
    void get(QString location);
    void post(QString location, QByteArray data);
    void updateDashboardLbl();
private slots:
    void readyRead();
    void arxivXmlReader();
    void openLink(QString text);
    void on_searchBtn_clicked();
//    void slotCustomContextMenumTblView(const QPoint pos);
    void slotCustomContextMenumListView(const QPoint& point);
    void slotEditLabel();
    void slotRmLabel();
    void saveAsActionSlotCell();
    void saveAsActionSlotRow();
    void on_actionExit_triggered();
    void setWindowStyle();
    void setMainPageTblModel();
    void setSingleSelectionViewStyle();
    void setClickableSignalDelegate();
    void setDashboardPageListModel();
    void on_lineEditSearch_returnPressed();
    void on_rmAllBtn_clicked();
    void on_cmbSearchItems_currentIndexChanged(int index);
    void on_searchResultView__doubleClicked(const QModelIndex &index);
    void on_plusBtn__clicked();
    void on_lblListView_doubleClicked(const QModelIndex &index);
    void currentRowLblChanged(const QModelIndex& currentIndex, const QModelIndex& prevIndex);

private:
    Ui::MainWindow *ui;
    QNetworkAccessManager* manager;
    QByteArray data;
    void initializeSearchComboBox();
    bool eventFilter(QObject* monitoredObj, QEvent* event);
    void initializeDashboard();
    void initializeConnections();
    void hanleSizeEventChng(QObject* monitoredObj, QEvent* event);
    void IconHandler(QLabel *monitoredObj, QString pixMapAddr);
    void searchViewColumnSizeHandler();
    void search(QString query, SearchQueryType type = SearchQueryType::NEW);
    AXTableModel* m_searchResultTblModel;
    AXQStringListModel* m_lblModel;
    QStringList m_labels;
    ClickableLinkDelegate* clickableDelegate;
    SearchFields m_searchField;
    QElapsedTimer m_timer;
    QString m_generatedQuery;
    QModelIndex m_homeIndex;
};
#endif // MAINWINDOW_H
