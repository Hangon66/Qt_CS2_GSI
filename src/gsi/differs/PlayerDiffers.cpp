#include "PlayerDiffers.h"
#include "GameState.h"
#include "GsiService.h"

namespace GSI {

// --- PlayerDiffer ---
void PlayerDiffer::diff(const GameState &prev, const GameState &curr, GsiService *e) {
    const auto &p = prev.player;
    const auto &c = curr.player;

    if (p.name != c.name)           emit e->playerNameChanged(p.name, c.name);
    if (p.clan != c.clan)           emit e->playerClanChanged(p.clan, c.clan);
    if (p.steamid != c.steamid)     emit e->playerSteamidChanged(p.steamid, c.steamid);
    if (p.team != c.team)           emit e->playerTeamChanged(p.team, c.team);
    if (p.activity != c.activity)   emit e->playerActivityChanged(p.activity, c.activity);
    if (p.observerSlot != c.observerSlot) emit e->playerObserverSlotChanged(p.observerSlot, c.observerSlot);
    if (p.specTarget != c.specTarget)     emit e->playerSpecTargetChanged(p.specTarget, c.specTarget);
    if (p.position != c.position)         emit e->playerPositionChanged(p.position, c.position);
    if (p.forwardDirection != c.forwardDirection) emit e->playerForwardChanged(p.forwardDirection, c.forwardDirection);
}

// --- PlayerStateDiffer ---
void PlayerStateDiffer::diff(const GameState &prev, const GameState &curr, GsiService *e) {
    // 当前玩家状态
    const auto &ps = prev.player.state;
    const auto &cs = curr.player.state;

    if (ps.health != cs.health)             emit e->playerHpChanged(ps.health, cs.health);
    if (ps.armor != cs.armor)               emit e->playerArmorChanged(ps.armor, cs.armor);
    if (ps.helmet != cs.helmet)             emit e->playerHelmetChanged(ps.helmet, cs.helmet);
    if (ps.money != cs.money)               emit e->playerMoneyChanged(ps.money, cs.money);
    if (ps.flashed != cs.flashed)           emit e->playerFlashedChanged(ps.flashed, cs.flashed);
    if (ps.smoked != cs.smoked)             emit e->playerSmokedChanged(ps.smoked, cs.smoked);
    if (ps.burning != cs.burning)           emit e->playerBurningChanged(ps.burning, cs.burning);
    if (ps.equipmentValue != cs.equipmentValue) emit e->playerEquipmentValueChanged(ps.equipmentValue, cs.equipmentValue);
    if (ps.roundKills != cs.roundKills)     emit e->playerRoundKillsChanged(ps.roundKills, cs.roundKills);

    // 新增字段 diff
    if (ps.roundHeadshotKills != cs.roundHeadshotKills)
        emit e->playerRoundHeadshotKillsChanged(ps.roundHeadshotKills, cs.roundHeadshotKills);
    if (ps.roundTotalDamage != cs.roundTotalDamage)
        emit e->playerRoundTotalDamageChanged(ps.roundTotalDamage, cs.roundTotalDamage);
    if (ps.defusekit != cs.defusekit)
        emit e->playerDefusekitChanged(ps.defusekit, cs.defusekit);

    // 语义推理
    if (ps.health > cs.health && cs.health >= 0)
        emit e->playerTookDamage(ps.health - cs.health);
    if (ps.health > 0 && cs.health == 0)
        emit e->playerDied();
    if (ps.health == 0 && cs.health > 0)
        emit e->playerRespawned();

    // allPlayers 状态
    const auto &prevIds = prev.allPlayers.list;
    const auto &currIds = curr.allPlayers.list;

    // 检查新增和变化的玩家
    for (auto it = currIds.begin(); it != currIds.end(); ++it) {
        const QString &sid = it.key();
        auto prevIt = prevIds.find(sid);

        if (prevIt == prevIds.end()) {
            // 新玩家加入
            emit e->allPlayersJoined(sid, it.value());
        } else {
            // 已有玩家状态变化
            const auto &pState = prevIt.value().state;
            const auto &cState = it.value().state;
            if (pState.health != cState.health)   emit e->allPlayersHpChanged(sid, pState.health, cState.health);
            if (pState.armor != cState.armor)      emit e->allPlayersArmorChanged(sid, pState.armor, cState.armor);
            if (pState.helmet != cState.helmet)    emit e->allPlayersHelmetChanged(sid, pState.helmet, cState.helmet);
            if (pState.money != cState.money)      emit e->allPlayersMoneyChanged(sid, pState.money, cState.money);
            if (pState.flashed != cState.flashed)  emit e->allPlayersFlashedChanged(sid, pState.flashed, cState.flashed);
            if (pState.smoked != cState.smoked)    emit e->allPlayersSmokedChanged(sid, pState.smoked, cState.smoked);
            if (pState.burning != cState.burning)  emit e->allPlayersBurningChanged(sid, pState.burning, cState.burning);

            // 新增 allPlayers 字段 diff
            if (pState.roundKills != cState.roundKills)
                emit e->allPlayersRoundKillsChanged(sid, pState.roundKills, cState.roundKills);
            if (pState.roundHeadshotKills != cState.roundHeadshotKills)
                emit e->allPlayersRoundHeadshotKillsChanged(sid, pState.roundHeadshotKills, cState.roundHeadshotKills);
            if (pState.equipmentValue != cState.equipmentValue)
                emit e->allPlayersEquipmentValueChanged(sid, pState.equipmentValue, cState.equipmentValue);
            if (pState.defusekit != cState.defusekit)
                emit e->allPlayersDefusekitChanged(sid, pState.defusekit, cState.defusekit);
        }
    }

    // 检查离开的玩家
    for (auto it = prevIds.begin(); it != prevIds.end(); ++it) {
        if (!currIds.contains(it.key())) {
            emit e->allPlayersLeft(it.key(), it.value());
        }
    }
}

// --- PlayerMatchStatsDiffer ---
void PlayerMatchStatsDiffer::diff(const GameState &prev, const GameState &curr, GsiService *e) {
    const auto &pm = prev.player.matchStats;
    const auto &cm = curr.player.matchStats;

    if (pm.kills != cm.kills)   emit e->playerKillsChanged(pm.kills, cm.kills);
    if (pm.deaths != cm.deaths) emit e->playerDeathsChanged(pm.deaths, cm.deaths);
    if (pm.assists != cm.assists) emit e->playerAssistsChanged(pm.assists, cm.assists);
    if (pm.score != cm.score)   emit e->playerScoreChanged(pm.score, cm.score);
    if (pm.mvps != cm.mvps)     emit e->playerMvpsChanged(pm.mvps, cm.mvps);

    // 语义推理：击杀
    if (cm.kills > pm.kills) {
        const Weapon *active = curr.player.weapons.getActive();
        emit e->playerGotKill(active ? active->name : QString());
    }

    // allPlayers 统计
    const auto &prevIds = prev.allPlayers.list;
    const auto &currIds = curr.allPlayers.list;
    for (auto it = currIds.begin(); it != currIds.end(); ++it) {
        const QString &sid = it.key();
        auto prevIt = prevIds.find(sid);
        if (prevIt != prevIds.end()) {
            const auto &pms = prevIt.value().matchStats;
            const auto &cms = it.value().matchStats;
            if (pms.kills != cms.kills)   emit e->allPlayersKillsChanged(sid, pms.kills, cms.kills);
            if (pms.deaths != cms.deaths) emit e->allPlayersDeathsChanged(sid, pms.deaths, cms.deaths);
            if (pms.assists != cms.assists) emit e->allPlayersAssistsChanged(sid, pms.assists, cms.assists);
            if (pms.score != cms.score)   emit e->allPlayersScoreChanged(sid, pms.score, cms.score);
            if (pms.mvps != cms.mvps)     emit e->allPlayersMvpsChanged(sid, pms.mvps, cms.mvps);
        }
    }
}

// --- WeaponDiffer ---
void WeaponDiffer::diff(const GameState &prev, const GameState &curr, GsiService *e) {
    // 当前武器变化
    const Weapon *prevActive = prev.player.weapons.getActive();
    const Weapon *currActive = curr.player.weapons.getActive();

    QString prevName = prevActive ? prevActive->name : QString();
    QString currName = currActive ? currActive->name : QString();

    if (prevName != currName) {
        Weapon emptyWeapon;
        emit e->playerWeaponChanged(prevActive ? *prevActive : emptyWeapon,
                                     currActive ? *currActive : emptyWeapon);
        emit e->playerActiveWeaponChanged(prevActive ? *prevActive : emptyWeapon,
                                           currActive ? *currActive : emptyWeapon);
    }

    // 弹药变化
    if (prevActive && currActive && prevActive->name == currActive->name) {
        if (prevActive->ammoClip != currActive->ammoClip)
            emit e->playerAmmoClipChanged(prevActive->ammoClip, currActive->ammoClip);
        if (prevActive->ammoReserve != currActive->ammoReserve)
            emit e->playerAmmoReserveChanged(prevActive->ammoReserve, currActive->ammoReserve);
    }

    // 武器列表增减检测
    QList<Weapon> pickedUp, dropped;
    for (const auto &w : curr.player.weapons.list) {
        bool found = false;
        for (const auto &pw : prev.player.weapons.list)
            if (pw.name == w.name) { found = true; break; }
        if (!found) pickedUp.append(w);
    }
    for (const auto &w : prev.player.weapons.list) {
        bool found = false;
        for (const auto &cw : curr.player.weapons.list)
            if (cw.name == w.name) { found = true; break; }
        if (!found) dropped.append(w);
    }
    if (!pickedUp.isEmpty()) emit e->playerWeaponsPickedUp(pickedUp);
    if (!dropped.isEmpty()) emit e->playerWeaponsDropped(dropped);

    // 武器列表整体变化
    if (prev.player.weapons != curr.player.weapons) {
        emit e->playerWeaponsChanged(prev.player.weapons, curr.player.weapons);
    }
}

} // namespace GSI
