#include "PlayerWidget.h"
#include "ui_PlayerWidget.h"
#include "gsi/GsiService.h"

PlayerWidget::PlayerWidget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::PlayerWidget)
{
    ui->setupUi(this);
}

PlayerWidget::~PlayerWidget() {
    delete ui;
}

void PlayerWidget::connectToService(GSI::GsiService *service) {
    if (!service) return;

    connect(service, &GSI::GsiService::playerNameChanged, this, [this](const QString &, const QString &curr) {
        ui->nameLabel->setText(QStringLiteral("玩家: %1").arg(curr));
    });

    connect(service, &GSI::GsiService::playerTeamChanged, this, [this](GSI::Team, GSI::Team curr) {
        ui->teamLabel->setText(QStringLiteral("队伍: %1").arg(GSI::teamToString(curr)));
    });

    connect(service, &GSI::GsiService::playerHpChanged, this, [this](int, int curr) {
        ui->hpLabel->setText(QStringLiteral("HP: %1").arg(curr));
        QString color = curr > 50 ? "green" : (curr > 20 ? "orange" : "red");
        ui->hpLabel->setStyleSheet(QStringLiteral("font-size: 16px; font-weight: bold; color: %1;").arg(color));
    });

    connect(service, &GSI::GsiService::playerArmorChanged, this, [this](int, int curr) {
        ui->armorLabel->setText(QStringLiteral("护甲: %1").arg(curr));
    });

    connect(service, &GSI::GsiService::playerMoneyChanged, this, [this](int, int curr) {
        ui->moneyLabel->setText(QStringLiteral("金钱: $%1").arg(curr));
    });

    connect(service, &GSI::GsiService::playerWeaponChanged, this, [this](const GSI::Weapon &, const GSI::Weapon &curr) {
        ui->weaponLabel->setText(QStringLiteral("武器: %1").arg(curr.displayName.isEmpty() ? curr.name : curr.displayName));
    });

    connect(service, &GSI::GsiService::playerAmmoClipChanged, this, [this](int, int curr) {
        ui->ammoLabel->setText(QStringLiteral("弹药: %1").arg(curr));
    });

    connect(service, &GSI::GsiService::playerKillsChanged, this, [this, service](int, int) {
        // 通过 lastState 获取完整 KDA
        const auto &state = service->lastState();
        ui->kdaLabel->setText(QStringLiteral("KDA: %1").arg(state.player.matchStats.kda()));
    });
}
