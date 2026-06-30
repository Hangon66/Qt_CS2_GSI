#include "LocalPlayerService.h"
#include "GsiService.h"

namespace GSI {

LocalPlayerService::LocalPlayerService(GsiService *service, QObject *parent)
    : QObject(parent)
    , m_service(service)
{
    connect(m_service, &GsiService::stateUpdated, this, &LocalPlayerService::onStateUpdated);

    // 连接炸弹语义事件（本地玩家安包/拆包）
    connect(m_service, &GsiService::bombPlanting, this, [this](const QString &sid) {
        if (m_service->isLocal(sid)) emit bombPlanting();
    });
    connect(m_service, &GsiService::bombPlanted, this, [this](const QString &sid) {
        if (m_service->isLocal(sid)) emit bombPlanted();
    });
}

const Player *LocalPlayerService::localPlayer() const {
    const auto &state = m_service->lastState();
    const QString &sid = state.provider.steamid;
    if (sid.isEmpty()) return nullptr;

    // 优先从 allPlayers 获取
    const Player *p = state.allPlayers.getBySteamid(sid);
    if (p) return p;

    // 回退到 player 字段（仅当它确实是本地玩家时）
    if (state.player.steamid == sid && !state.player.name.isEmpty()) {
        return &state.player;
    }

    return nullptr;
}

bool LocalPlayerService::isAvailable() const {
    return localPlayer() != nullptr;
}

void LocalPlayerService::onStateUpdated() {
    const auto &state = m_service->lastState();
    const QString &sid = state.provider.steamid;
    if (sid.isEmpty()) return;

    // 尝试从 allPlayers 获取本地玩家
    const Player *currLocal = state.allPlayers.getBySteamid(sid);
    bool currAvailable = (currLocal != nullptr);

    if (currAvailable) {
        if (!m_wasAvailable) {
            // 首次出现
            emit localPlayerJoined(*currLocal);
            m_lastLocal = *currLocal;
            m_wasAvailable = true;
            return;
        }

        // 逐字段对比
        compareAndEmit(m_lastLocal, *currLocal);
        m_lastLocal = *currLocal;
        return;
    }

    // allPlayers 中无本地玩家，尝试回退到 player 字段
    // 仅当 player.steamid == provider.steamid 时才是本地玩家（观战时不相等）
    if (state.player.steamid == sid && !state.player.name.isEmpty()) {
        if (!m_wasAvailable) {
            emit localPlayerJoined(state.player);
            m_lastLocal = state.player;
            m_wasAvailable = true;
            return;
        }

        compareAndEmit(m_lastLocal, state.player);
        m_lastLocal = state.player;
        return;
    }

    // 本地玩家不可用
    if (m_wasAvailable) {
        emit localPlayerLeft(m_lastLocal);
        m_wasAvailable = false;
        m_lastLocal = Player();
    }
}

void LocalPlayerService::compareAndEmit(const Player &p, const Player &c) {
    // 基础信息
    if (p.name != c.name)             emit nameChanged(p.name, c.name);
    if (p.team != c.team)             emit teamChanged(p.team, c.team);

    // 实时状态
    if (p.state.health != c.state.health)         emit hpChanged(p.state.health, c.state.health);
    if (p.state.armor != c.state.armor)           emit armorChanged(p.state.armor, c.state.armor);
    if (p.state.helmet != c.state.helmet)         emit helmetChanged(p.state.helmet, c.state.helmet);
    if (p.state.money != c.state.money)           emit moneyChanged(p.state.money, c.state.money);
    if (p.state.flashed != c.state.flashed)       emit flashedChanged(p.state.flashed, c.state.flashed);
    if (p.state.smoked != c.state.smoked)         emit smokedChanged(p.state.smoked, c.state.smoked);
    if (p.state.burning != c.state.burning)       emit burningChanged(p.state.burning, c.state.burning);
    if (p.state.defusekit != c.state.defusekit)   emit defusekitChanged(p.state.defusekit, c.state.defusekit);
    if (p.state.roundKills != c.state.roundKills)
        emit roundKillsChanged(p.state.roundKills, c.state.roundKills);
    if (p.state.roundHeadshotKills != c.state.roundHeadshotKills)
        emit roundHeadshotKillsChanged(p.state.roundHeadshotKills, c.state.roundHeadshotKills);
    if (p.state.roundTotalDamage != c.state.roundTotalDamage)
        emit roundTotalDamageChanged(p.state.roundTotalDamage, c.state.roundTotalDamage);

    // 语义推理：受伤/死亡/重生
    if (p.state.health > c.state.health && c.state.health >= 0)
        emit tookDamage(p.state.health - c.state.health);
    if (p.state.health > 0 && c.state.health == 0)
        emit died();
    if (p.state.health == 0 && c.state.health > 0)
        emit respawned();

    // 比赛统计
    if (p.matchStats.kills != c.matchStats.kills)     emit killsChanged(p.matchStats.kills, c.matchStats.kills);
    if (p.matchStats.deaths != c.matchStats.deaths)   emit deathsChanged(p.matchStats.deaths, c.matchStats.deaths);
    if (p.matchStats.assists != c.matchStats.assists) emit assistsChanged(p.matchStats.assists, c.matchStats.assists);
    if (p.matchStats.score != c.matchStats.score)     emit scoreChanged(p.matchStats.score, c.matchStats.score);
    if (p.matchStats.mvps != c.matchStats.mvps)       emit mvpsChanged(p.matchStats.mvps, c.matchStats.mvps);

    // 语义推理：击杀
    if (c.matchStats.kills > p.matchStats.kills) {
        const Weapon *active = c.weapons.getActive();
        emit gotKill(active ? active->name : QString());
    }

    // 武器
    compareWeapons(p, c);
}

void LocalPlayerService::compareWeapons(const Player &p, const Player &c) {
    const Weapon *prevActive = p.weapons.getActive();
    const Weapon *currActive = c.weapons.getActive();

    // 活跃武器名称变化 → weaponChanged
    QString prevName = prevActive ? prevActive->name : QString();
    QString currName = currActive ? currActive->name : QString();

    if (prevName != currName) {
        Weapon emptyWeapon;
        emit weaponChanged(prevActive ? *prevActive : emptyWeapon,
                           currActive ? *currActive : emptyWeapon);
    }

    // 同一武器的弹药变化
    if (prevActive && currActive && prevActive->name == currActive->name) {
        if (prevActive->ammoClip != currActive->ammoClip)
            emit ammoClipChanged(prevActive->ammoClip, currActive->ammoClip);
        if (prevActive->ammoReserve != currActive->ammoReserve)
            emit ammoReserveChanged(prevActive->ammoReserve, currActive->ammoReserve);
    }
}

} // namespace GSI
