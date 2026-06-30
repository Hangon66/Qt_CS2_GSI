#include "MapWidget.h"
#include "ui_MapWidget.h"
#include "gsi/GsiService.h"

// 地图阶段中文名
static QString mapPhaseName(GSI::MapPhase phase) {
    using P = GSI::MapPhase;
    switch (phase) {
    case P::Warmup:       return QStringLiteral("热身");
    case P::Live:         return QStringLiteral("进行中");
    case P::Intermission: return QStringLiteral("休息");
    case P::GameOver:     return QStringLiteral("比赛结束");
    default:              return QStringLiteral("--");
    }
}

// 倒计时阶段中文名
static QString countdownPhaseName(GSI::CountdownPhase phase) {
    using P = GSI::CountdownPhase;
    switch (phase) {
    case P::Warmup:     return QStringLiteral("热身");
    case P::Freezetime: return QStringLiteral("冻结时间");
    case P::Live:       return QStringLiteral("比赛中");
    case P::Bomb:       return QStringLiteral("炸弹倒计时");
    case P::Defusing:   return QStringLiteral("拆弹倒计时");
    case P::Paused:     return QStringLiteral("暂停");
    case P::Timeout:    return QStringLiteral("战术暂停");
    case P::Over:       return QStringLiteral("回合结束");
    default:            return QStringLiteral("--");
    }
}

// 倒计时阶段样式
static QString countdownPhaseStyle(GSI::CountdownPhase phase) {
    using P = GSI::CountdownPhase;
    switch (phase) {
    case P::Bomb:       return QStringLiteral("color: red; font-size: 16px; font-weight: bold;");
    case P::Defusing:   return QStringLiteral("color: #00BFFF; font-size: 16px; font-weight: bold;");
    case P::Freezetime: return QStringLiteral("color: orange; font-weight: bold;");
    case P::Paused:     return QStringLiteral("color: gray; font-style: italic;");
    case P::Timeout:    return QStringLiteral("color: #9932CC; font-weight: bold;");
    case P::Over:       return QStringLiteral("color: gray;");
    default:            return QStringLiteral("");
    }
}

// 格式化秒数为 M:SS
static QString formatTime(int totalSeconds) {
    int m = totalSeconds / 60;
    int s = totalSeconds % 60;
    return QStringLiteral("%1:%2").arg(m).arg(s, 2, 10, QLatin1Char('0'));
}

MapWidget::MapWidget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::MapWidget)
    , m_countdownTimer(new QTimer(this))
{
    ui->setupUi(this);
    connect(m_countdownTimer, &QTimer::timeout, this, &MapWidget::onEstimatedTick);
}

MapWidget::~MapWidget() {
    delete ui;
}

void MapWidget::updateCountdownLabel() {
    QString text = QStringLiteral("%1: %2").arg(countdownPhaseName(m_countdownPhase), m_countdownTime);
    ui->countdownLabel->setText(text);
    ui->countdownLabel->setStyleSheet(countdownPhaseStyle(m_countdownPhase));
}

void MapWidget::startEstimatedCountdown(EstimateType type, int seconds) {
    if (m_spectatorMode) return;
    m_estimateType = type;
    m_estimatedSeconds = seconds;
    m_countdownTimer->start(1000);

    // 立即更新显示
    QString name;
    QString style;
    switch (type) {
    case EstimateType::Freezetime:
        name = QStringLiteral("冻结时间");
        style = QStringLiteral("color: orange; font-weight: bold;");
        break;
    case EstimateType::Round:
        name = QStringLiteral("回合时间");
        style = QStringLiteral("");
        break;
    case EstimateType::Bomb:
        name = QStringLiteral("炸弹倒计时");
        style = QStringLiteral("color: red; font-size: 16px; font-weight: bold;");
        break;
    default:
        break;
    }
    ui->countdownLabel->setText(QStringLiteral("%1: %2").arg(name, formatTime(m_estimatedSeconds)));
    ui->countdownLabel->setStyleSheet(style);
}

void MapWidget::stopEstimatedCountdown() {
    if (m_spectatorMode) return;
    m_countdownTimer->stop();
    m_estimateType = EstimateType::None;
    m_estimatedSeconds = 0;
    ui->countdownLabel->setText(QStringLiteral("倒计时: --"));
    ui->countdownLabel->setStyleSheet(QStringLiteral(""));
}

void MapWidget::onEstimatedTick() {
    m_estimatedSeconds--;
    if (m_estimatedSeconds < 0) {
        stopEstimatedCountdown();
        return;
    }

    QString name;
    QString style;
    switch (m_estimateType) {
    case EstimateType::Freezetime:
        name = QStringLiteral("冻结时间");
        style = QStringLiteral("color: orange; font-weight: bold;");
        break;
    case EstimateType::Round:
        name = QStringLiteral("回合时间");
        style = QStringLiteral("");
        break;
    case EstimateType::Bomb:
        name = QStringLiteral("炸弹倒计时");
        style = QStringLiteral("color: red; font-size: 16px; font-weight: bold;");
        if (m_estimatedSeconds <= 10)
            style = QStringLiteral("color: red; font-size: 18px; font-weight: bold;");
        break;
    default:
        break;
    }
    ui->countdownLabel->setText(QStringLiteral("%1: %2").arg(name, formatTime(m_estimatedSeconds)));
    ui->countdownLabel->setStyleSheet(style);
}

void MapWidget::connectToService(GSI::GsiService *service) {
    if (!service) return;

    connect(service, &GSI::GsiService::mapNameChanged, this, [this](const QString &, const QString &curr) {
        ui->mapNameLabel->setText(QStringLiteral("地图: %1").arg(curr));
    });

    connect(service, &GSI::GsiService::mapPhaseChanged, this, [this](GSI::MapPhase, GSI::MapPhase curr) {
        ui->phaseLabel->setText(QStringLiteral("阶段: %1").arg(mapPhaseName(curr)));
    });

    connect(service, &GSI::GsiService::mapRoundChanged, this, [this](int, int curr) {
        ui->roundLabel->setText(QStringLiteral("回合: %1").arg(curr));
    });

    connect(service, &GSI::GsiService::mapTeamCTScoreChanged, this, [this](int, int curr) {
        ui->ctScoreLabel->setText(QStringLiteral("CT: %1").arg(curr));
    });

    connect(service, &GSI::GsiService::mapTeamTScoreChanged, this, [this](int, int curr) {
        ui->tScoreLabel->setText(QStringLiteral("T: %1").arg(curr));
    });

    // ====== 估算倒计时（非观战模式） ======

    // 回合阶段变化 → 启动/停止估算倒计时
    connect(service, &GSI::GsiService::roundPhaseChanged, this,
            [this](GSI::RoundPhase, GSI::RoundPhase curr) {
        if (m_spectatorMode) return;
        switch (curr) {
        case GSI::RoundPhase::Freezetime:
            startEstimatedCountdown(EstimateType::Freezetime, 15);
            break;
        case GSI::RoundPhase::Live:
            startEstimatedCountdown(EstimateType::Round, 115);
            break;
        case GSI::RoundPhase::Over:
            stopEstimatedCountdown();
            break;
        default:
            break;
        }
    });

    // 炸弹状态变化（round.bomb，非观战可用） → 启动/停止炸弹倒计时
    connect(service, &GSI::GsiService::roundBombChanged, this,
            [this](GSI::StableBombState, GSI::StableBombState curr) {
        if (m_spectatorMode) return;
        switch (curr) {
        case GSI::StableBombState::Planted:
            startEstimatedCountdown(EstimateType::Bomb, 40);
            break;
        case GSI::StableBombState::Exploded:
        case GSI::StableBombState::Defused:
            stopEstimatedCountdown();
            break;
        default:
            break;
        }
    });

    // ====== 观战模式精确倒计时 ======

    // 倒计时阶段变化（仅观战模式）
    connect(service, &GSI::GsiService::phaseCountdownsPhaseChanged, this,
            [this](GSI::CountdownPhase, GSI::CountdownPhase curr) {
        m_spectatorMode = true;
        m_countdownTimer->stop();
        m_countdownPhase = curr;
        updateCountdownLabel();
    });

    // 倒计时时间变化（仅观战模式）
    connect(service, &GSI::GsiService::phaseCountdownsTimeChanged, this,
            [this](const QString &, const QString &curr) {
        m_spectatorMode = true;
        m_countdownTime = curr;
        updateCountdownLabel();
    });
}
