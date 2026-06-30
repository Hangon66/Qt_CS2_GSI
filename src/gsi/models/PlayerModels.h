#ifndef PLAYERMODELS_H
#define PLAYERMODELS_H

#include <QString>
#include <QJsonObject>
#include <QList>
#include "GsiEnums.h"
#include "Vector3D.h"

namespace GSI {

/**
 * @brief 玩家状态（HP、护甲、金钱等）
 */
struct PlayerState {
    int health = 0;
    int armor = 0;
    bool helmet = false;
    int flashed = 0;
    int smoked = 0;
    int burning = 0;
    int money = 0;
    int roundKills = 0;
    int roundHeadshotKills = 0;
    int equipmentValue = 0;

    PlayerState() = default;
    explicit PlayerState(const QJsonObject &obj);
    QJsonObject toJsonObject() const;
    bool operator==(const PlayerState &other) const;
    bool operator!=(const PlayerState &other) const { return !(*this == other); }
    bool isAlive() const { return health > 0; }
    bool hasArmor() const { return armor > 0 || helmet; }
};

/**
 * @brief 玩家比赛统计（KDA 等）
 */
struct PlayerMatchStats {
    int kills = 0;
    int assists = 0;
    int deaths = 0;
    int mvps = 0;
    int score = 0;

    PlayerMatchStats() = default;
    explicit PlayerMatchStats(const QJsonObject &obj);
    QJsonObject toJsonObject() const;
    bool operator==(const PlayerMatchStats &other) const;
    bool operator!=(const PlayerMatchStats &other) const { return !(*this == other); }
    QString kda() const;
};

/**
 * @brief 武器数据
 */
struct Weapon {
    QString name;
    WeaponState state = WeaponState::Unknown;
    WeaponType type = WeaponType::Unknown;
    QString displayName;
    int ammoClip = -1;
    int ammoClipMax = -1;
    int ammoReserve = -1;

    Weapon() = default;
    explicit Weapon(const QJsonObject &obj);
    QJsonObject toJsonObject() const;
    bool operator==(const Weapon &other) const;
    bool operator!=(const Weapon &other) const { return !(*this == other); }
    bool isActive() const { return state == WeaponState::Active; }
    bool isGrenade() const { return type == WeaponType::Grenade; }
    bool isC4() const { return name == "weapon_c4"; }
    bool isPrimary() const;
    bool isSecondary() const { return type == WeaponType::Pistol; }
    bool isMelee() const { return type == WeaponType::Knife; }
};

/**
 * @brief 武器集合
 */
struct WeaponsCollection {
    QList<Weapon> list;

    WeaponsCollection() = default;
    explicit WeaponsCollection(const QJsonObject &obj);
    QJsonObject toJsonObject() const;
    bool operator==(const WeaponsCollection &other) const;
    bool operator!=(const WeaponsCollection &other) const { return !(*this == other); }
    const Weapon *getActive() const;
    QList<Weapon> getGrenades() const;
    bool hasC4() const;
};

/**
 * @brief 玩家数据
 */
struct Player {
    QString steamid;
    QString name;
    QString clan;
    int xpOverloadLevel = 0;
    Team team = Team::Unknown;
    Activity activity = Activity::Unknown;
    int observerSlot = -1;
    QString specTarget;
    Vector3D position;
    Vector3D forwardDirection;
    PlayerState state;
    PlayerMatchStats matchStats;
    WeaponsCollection weapons;
    const Weapon *activeWeapon = nullptr;

    Player() = default;
    explicit Player(const QJsonObject &obj);
    QJsonObject toJsonObject() const;
    bool operator==(const Player &other) const;
    bool operator!=(const Player &other) const { return !(*this == other); }
};

} // namespace GSI

#endif // PLAYERMODELS_H
