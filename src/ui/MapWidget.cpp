#include "MapWidget.h"
#include "ui_MapWidget.h"
#include "gsi/GsiService.h"

MapWidget::MapWidget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::MapWidget)
{
    ui->setupUi(this);
}

MapWidget::~MapWidget() {
    delete ui;
}

void MapWidget::connectToService(GSI::GsiService *service) {
    if (!service) return;

    connect(service, &GSI::GsiService::mapNameChanged, this, [this](const QString &, const QString &curr) {
        ui->mapNameLabel->setText(QStringLiteral("地图: %1").arg(curr));
    });

    connect(service, &GSI::GsiService::mapPhaseChanged, this, [this](GSI::MapPhase, GSI::MapPhase curr) {
        ui->phaseLabel->setText(QStringLiteral("阶段: %1").arg(GSI::mapPhaseToString(curr)));
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

    connect(service, &GSI::GsiService::phaseCountdownsTimeChanged, this, [this](const QString &, const QString &curr) {
        ui->countdownLabel->setText(QStringLiteral("倒计时: %1").arg(curr));
    });
}
