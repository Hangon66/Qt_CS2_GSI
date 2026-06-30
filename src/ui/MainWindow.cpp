#include "MainWindow.h"
#include "ui_MainWindow.h"
#include "MapWidget.h"
#include "PlayerWidget.h"
#include "EventLogWidget.h"
#include "GsiService.h"

#include <QVBoxLayout>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // 创建子面板并放入容器
    m_mapWidget = new MapWidget(this);
    auto *mapLayout = new QVBoxLayout(ui->mapContainer);
    mapLayout->setContentsMargins(0, 0, 0, 0);
    mapLayout->addWidget(m_mapWidget);

    m_playerWidget = new PlayerWidget(this);
    auto *playerLayout = new QVBoxLayout(ui->playerContainer);
    playerLayout->setContentsMargins(0, 0, 0, 0);
    playerLayout->addWidget(m_playerWidget);

    m_eventLogWidget = new EventLogWidget(this);
    auto *logLayout = new QVBoxLayout(ui->eventLogContainer);
    logLayout->setContentsMargins(0, 0, 0, 0);
    logLayout->addWidget(m_eventLogWidget);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::setService(GSI::GsiService *service)
{
    m_service = service;

    // 将 GsiService 连接到各面板
    m_mapWidget->connectToService(service);
    m_playerWidget->connectToService(service);
    m_eventLogWidget->connectToService(service);

    // 更新状态标签
    connect(service, &GSI::GsiService::rawUpdate, this, [this](const QJsonObject &) {
        ui->statusLabel->setText(QStringLiteral("GSI 服务运行中 — 已接收数据"));
        ui->statusLabel->setStyleSheet(QStringLiteral("color: green; font-size: 12px; padding: 4px;"));
    });

    ui->statusLabel->setText(QStringLiteral("GSI 服务已启动，等待 CS2 推送数据..."));
    ui->statusLabel->setStyleSheet(QStringLiteral("color: blue; font-size: 12px; padding: 4px;"));
}
