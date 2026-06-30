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
    /**
     * @brief 生命值，0 表示死亡。
     */
    int health = 0;
    /**
     * @brief 护甲值 (0-100)。
     */
    int armor = 0;
    /**
     * @brief 是否拥有头盔。
     */
    bool helmet = false;
    /**
     * @brief 闪光效果强度 (0-255)。
     */
    int flashed = 0;
    /**
     * @brief 烟雾效果强度 (0-255)。
     */
    int smoked = 0;
    /**
     * @brief 燃烧效果强度 (0-255)。
     */
    int burning = 0;
    /**
     * @brief 当前金钱。
     */
    int money = 0;
    /**
     * @brief 本回合击杀数。
     */
    int roundKills = 0;
    /**
     * @brief 本回合爆头击杀数。
     */
    int roundHeadshotKills = 0;
    /**
     * @brief 当前装备总价值。
     */
    int equipmentValue = 0;
    /**
     * @brief 是否拥有拆弹器 (GSI: "defusekit")。
     */
    bool defusekit = false;
    /**
     * @brief 本回合总伤害 (GSI: "round_totaldmg")。
     */
    int roundTotalDamage = 0;

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
    /**
     * @brief 本局总击杀数。
     */
    int kills = 0;
    /**
     * @brief 本局总助攻数。
     */
    int assists = 0;
    /**
     * @brief 本局总死亡数。
     */
    int deaths = 0;
    /**
     * @brief 本局 MVP 次数。
     */
    int mvps = 0;
    /**
     * @brief 本局总得分。
     */
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
    /**
     * @brief 武器内部名称 (如 "weapon_ak47")。
     */
    QString name;
    /**
     * @brief 武器当前状态 (Active / Holstered / Reloading)。
     */
    WeaponState state = WeaponState::Unknown;
    /**
     * @brief 武器类型 (Rifle / Pistol / Grenade 等)。
     */
    WeaponType type = WeaponType::Unknown;
    /**
     * @brief 武器显示名称（从 WeaponData 查询表获取）。
     */
    QString displayName;
    /**
     * @brief 弹匣内弹药数，-1 表示未知。
     */
    int ammoClip = -1;
    /**
     * @brief 弹匣最大容量，-1 表示未知。
     */
    int ammoClipMax = -1;
    /**
     * @brief 备弹量，-1 表示未知。
     */
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
    /**
     * @brief 武器列表，包含玩家拥有的所有武器。
     */
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
    /**
     * @brief 玩家的 SteamID64。
     */
    QString steamid;
    /**
     * @brief 玩家名称。
     */
    QString name;
    /**
     * @brief 战队标签。
     */
    QString clan;
    /**
     * @brief 经验超载等级。
     */
    int xpOverloadLevel = 0;
    /**
     * @brief 所属队伍 (CT / T / Spectator)。
     */
    Team team = Team::Unknown;
    /**
     * @brief 活动状态 (Playing / Menu / TextInput)。
     */
    Activity activity = Activity::Unknown;
    /**
     * @brief 观察者槽位，-1 表示非观察者。
     */
    int observerSlot = -1;
    /**
     * @brief 观战目标的 SteamID64。
     */
    QString specTarget;
    /**
     * @brief 玩家世界坐标（仅观察者可见）。
     */
    Vector3D position;
    /**
     * @brief 玩家朝向向量（仅观察者可见）。
     */
    Vector3D forwardDirection;
    /**
     * @brief 玩家实时状态（HP、护甲、金钱等）。
     */
    PlayerState state;
    /**
     * @brief 玩家本局比赛统计（KDA 等）。
     */
    PlayerMatchStats matchStats;
    /**
     * @brief 玩家武器集合。
     */
    WeaponsCollection weapons;
    /**
     * @brief 当前活跃武器指针，可能为 nullptr。
     */
    const Weapon *activeWeapon = nullptr;

    Player() = default;
    explicit Player(const QJsonObject &obj);
    QJsonObject toJsonObject() const;
    bool operator==(const Player &other) const;
    bool operator!=(const Player &other) const { return !(*this == other); }
};

} // namespace GSI

#endif // PLAYERMODELS_H
