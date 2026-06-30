#include "PlayerWidget.h"
#include "ui_PlayerWidget.h"
#include "gsi/GsiService.h"
#include "gsi/LocalPlayerService.h"
#include "utils/SoundPlayer.h"

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
    auto *lp = service->localPlayer();
    if (!lp) return;

    // ====== 首次数据填充：玩家首次出现时一次性设置所有标签 ======
    connect(lp, &GSI::LocalPlayerService::localPlayerJoined, this,
            [this, lp](const GSI::Player &) {
        const auto *p = lp->localPlayer();
        if (!p) return;

        // 名称
        ui->nameLabel->setText(QStringLiteral("玩家: %1").arg(p->name));
        // 队伍
        ui->teamLabel->setText(QStringLiteral("队伍: %1").arg(GSI::teamToString(p->team)));
        // HP
        ui->hpLabel->setText(QStringLiteral("HP: %1").arg(p->state.health));
        {
            int hp = p->state.health;
            QString color = hp > 50 ? "green" : (hp > 20 ? "orange" : "red");
            ui->hpLabel->setStyleSheet(QStringLiteral("font-size: 16px; font-weight: bold; color: %1;").arg(color));
        }
        // 护甲
        ui->armorLabel->setText(QStringLiteral("护甲: %1").arg(p->state.armor));
        // 金钱
        ui->moneyLabel->setText(QStringLiteral("金钱: $%1").arg(p->state.money));
        // 武器
        const auto *active = p->weapons.getActive();
        if (active)
            ui->weaponLabel->setText(QStringLiteral("武器: %1").arg(active->displayName.isEmpty() ? active->name : active->displayName));
        // 弹药
        if (active)
            ui->ammoLabel->setText(QStringLiteral("弹药: %1").arg(active->ammoClip));
        // 状态
        if (p->state.health > 0) {
            ui->statusLabel->setText(QStringLiteral("状态: 存活"));
            ui->statusLabel->setStyleSheet(QStringLiteral("color: green; font-weight: bold;"));
        } else {
            ui->statusLabel->setText(QStringLiteral("状态: 死亡"));
            ui->statusLabel->setStyleSheet(QStringLiteral("color: red; font-weight: bold;"));
        }
        // KDA
        ui->kdaLabel->setText(QStringLiteral("KDA: %1/%2/%3")
            .arg(p->matchStats.kills).arg(p->matchStats.deaths).arg(p->matchStats.assists));
    });

    // ====== 后续变化监听 ======

    // 名称
    connect(lp, &GSI::LocalPlayerService::nameChanged, this,
            [this](const QString &, const QString &curr) {
        ui->nameLabel->setText(QStringLiteral("玩家: %1").arg(curr));
    });

    // 队伍
    connect(lp, &GSI::LocalPlayerService::teamChanged, this,
            [this](GSI::Team, GSI::Team curr) {
        ui->teamLabel->setText(QStringLiteral("队伍: %1").arg(GSI::teamToString(curr)));
    });

    // HP
    connect(lp, &GSI::LocalPlayerService::hpChanged, this,
            [this](int, int curr) {
        ui->hpLabel->setText(QStringLiteral("HP: %1").arg(curr));
        QString color = curr > 50 ? "green" : (curr > 20 ? "orange" : "red");
        ui->hpLabel->setStyleSheet(QStringLiteral("font-size: 16px; font-weight: bold; color: %1;").arg(color));
    });

    // 护甲
    connect(lp, &GSI::LocalPlayerService::armorChanged, this,
            [this](int, int curr) {
        ui->armorLabel->setText(QStringLiteral("护甲: %1").arg(curr));
    });

    // 金钱
    connect(lp, &GSI::LocalPlayerService::moneyChanged, this,
            [this](int, int curr) {
        ui->moneyLabel->setText(QStringLiteral("金钱: $%1").arg(curr));
    });

    // 武器
    connect(lp, &GSI::LocalPlayerService::weaponChanged, this,
            [this](const GSI::Weapon &, const GSI::Weapon &curr) {
        ui->weaponLabel->setText(QStringLiteral("武器: %1").arg(curr.displayName.isEmpty() ? curr.name : curr.displayName));
    });

    // 弹药
    connect(lp, &GSI::LocalPlayerService::ammoClipChanged, this,
            [this](int, int curr) {
        ui->ammoLabel->setText(QStringLiteral("弹药: %1").arg(curr));
    });

    // KDA
    auto updateKda = [this, lp](int, int) {
        const auto *p = lp->localPlayer();
        if (p) {
            ui->kdaLabel->setText(QStringLiteral("KDA: %1/%2/%3")
                .arg(p->matchStats.kills).arg(p->matchStats.deaths).arg(p->matchStats.assists));
        }
    };
    connect(lp, &GSI::LocalPlayerService::killsChanged, this, updateKda);
    connect(lp, &GSI::LocalPlayerService::deathsChanged, this, updateKda);
    connect(lp, &GSI::LocalPlayerService::assistsChanged, this, updateKda);

    // 死亡/重生
    connect(lp, &GSI::LocalPlayerService::died, this, [this]() {
        ui->statusLabel->setText(QStringLiteral("状态: 死亡"));
        ui->statusLabel->setStyleSheet(QStringLiteral("color: red; font-weight: bold; font-size: 16px;"));
        ui->hpLabel->setStyleSheet(QStringLiteral("font-size: 16px; font-weight: bold; color: gray;"));
        SoundPlayer::playDeath();
    });

    connect(lp, &GSI::LocalPlayerService::respawned, this, [this]() {
        ui->statusLabel->setText(QStringLiteral("状态: 存活"));
        ui->statusLabel->setStyleSheet(QStringLiteral("color: green; font-weight: bold;"));
    });
}
