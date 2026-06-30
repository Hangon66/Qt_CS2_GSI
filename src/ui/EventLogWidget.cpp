#include "EventLogWidget.h"
#include "ui_EventLogWidget.h"
#include "gsi/GsiService.h"
#include <QDateTime>

EventLogWidget::EventLogWidget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::EventLogWidget)
{
    ui->setupUi(this);
    connect(ui->clearButton, &QPushButton::clicked, this, &EventLogWidget::onClearClicked);
}

EventLogWidget::~EventLogWidget() {
    delete ui;
}

void EventLogWidget::connectToService(GSI::GsiService *service) {
    if (!service) return;

    // 连接 rawUpdate 信号来记录所有原始更新
    connect(service, &GSI::GsiService::rawUpdate, this, [this](const QJsonObject &json) {
        QString provider = json.value("provider").toObject().value("name").toString();
        appendLog(QStringLiteral("收到 GSI 更新 (provider: %1, keys: %2)")
                      .arg(provider, QString::number(json.keys().size())));
    });

    // 连接关键事件信号
    connect(service, &GSI::GsiService::playerHpChanged, this, [this](int prev, int curr) {
        appendLog(QStringLiteral("玩家 HP: %1 → %2").arg(prev).arg(curr));
    });

    connect(service, &GSI::GsiService::playerWeaponChanged, this, [this](const GSI::Weapon &prev, const GSI::Weapon &curr) {
        appendLog(QStringLiteral("武器切换: %1 → %2").arg(prev.name, curr.name));
    });

    connect(service, &GSI::GsiService::mapPhaseChanged, this, [this](GSI::MapPhase prev, GSI::MapPhase curr) {
        appendLog(QStringLiteral("地图阶段: %1 → %2")
                      .arg(GSI::mapPhaseToString(prev), GSI::mapPhaseToString(curr)));
    });

    connect(service, &GSI::GsiService::roundPhaseChanged, this, [this](GSI::RoundPhase prev, GSI::RoundPhase curr) {
        appendLog(QStringLiteral("回合阶段: %1 → %2")
                      .arg(GSI::roundPhaseToString(prev), GSI::roundPhaseToString(curr)));
    });

    connect(service, &GSI::GsiService::roundStarted, this, [this]() {
        appendLog(QStringLiteral(">>> 回合开始 <<<"));
    });

    connect(service, &GSI::GsiService::bombStateChanged, this, [this](GSI::BombState prev, GSI::BombState curr) {
        appendLog(QStringLiteral("炸弹状态: %1 → %2")
                      .arg(GSI::bombStateToString(prev), GSI::bombStateToString(curr)));
    });

    connect(service, &GSI::GsiService::mapTeamCTScoreChanged, this, [this](int prev, int curr) {
        appendLog(QStringLiteral("CT 比分: %1 → %2").arg(prev).arg(curr));
    });

    connect(service, &GSI::GsiService::mapTeamTScoreChanged, this, [this](int prev, int curr) {
        appendLog(QStringLiteral("T 比分: %1 → %2").arg(prev).arg(curr));
    });
}

void EventLogWidget::onClearClicked() {
    ui->logTextEdit->clear();
}

void EventLogWidget::appendLog(const QString &msg) {
    const QString timestamp = QDateTime::currentDateTime().toString("hh:mm:ss.zzz");
    ui->logTextEdit->append(QStringLiteral("[%1] %2").arg(timestamp, msg));
}
