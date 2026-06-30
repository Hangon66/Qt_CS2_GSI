#ifndef GSIENUMS_H
#define GSIENUMS_H

#include <QString>
#include <QRegularExpression>

namespace GSI {

/**
 * @brief 队伍枚举
 */
enum class Team {
    Unknown,
    CT,
    T
};

/**
 * @brief 将字符串转换为 Team 枚举
 * @param str GSI JSON 中的队伍字符串 ("CT" / "T")
 * @return 对应的 Team 枚举值
 */
inline Team stringToTeam(const QString &str) {
    if (str == "CT") return Team::CT;
    if (str == "T") return Team::T;
    return Team::Unknown;
}

inline QString teamToString(Team team) {
    switch (team) {
        case Team::CT: return QStringLiteral("CT");
        case Team::T: return QStringLiteral("T");
        default: return QStringLiteral("unknown");
    }
}

/**
 * @brief 玩家活动状态枚举
 */
enum class Activity {
    Unknown,
    Menu,
    Playing,
    TextInput
};

inline Activity stringToActivity(const QString &str) {
    if (str == "menu") return Activity::Menu;
    if (str == "playing") return Activity::Playing;
    if (str == "textinput") return Activity::TextInput;
    return Activity::Unknown;
}

inline QString activityToString(Activity activity) {
    switch (activity) {
        case Activity::Menu: return QStringLiteral("menu");
        case Activity::Playing: return QStringLiteral("playing");
        case Activity::TextInput: return QStringLiteral("textinput");
        default: return QStringLiteral("unknown");
    }
}

/**
 * @brief 回合阶段枚举
 */
enum class RoundPhase {
    Unknown,
    Warmup,
    Live,
    Over,
    Freezetime
};

inline RoundPhase stringToRoundPhase(const QString &str) {
    if (str == "warmup") return RoundPhase::Warmup;
    if (str == "live") return RoundPhase::Live;
    if (str == "over") return RoundPhase::Over;
    if (str == "freezetime") return RoundPhase::Freezetime;
    return RoundPhase::Unknown;
}

inline QString roundPhaseToString(RoundPhase phase) {
    switch (phase) {
        case RoundPhase::Warmup: return QStringLiteral("warmup");
        case RoundPhase::Live: return QStringLiteral("live");
        case RoundPhase::Over: return QStringLiteral("over");
        case RoundPhase::Freezetime: return QStringLiteral("freezetime");
        default: return QStringLiteral("unknown");
    }
}

/**
 * @brief 地图阶段枚举
 */
enum class MapPhase {
    Unknown,
    Warmup,
    Live,
    Intermission,
    GameOver
};

inline MapPhase stringToMapPhase(const QString &str) {
    if (str == "warmup") return MapPhase::Warmup;
    if (str == "live") return MapPhase::Live;
    if (str == "intermission") return MapPhase::Intermission;
    if (str == "gameover") return MapPhase::GameOver;
    return MapPhase::Unknown;
}

inline QString mapPhaseToString(MapPhase phase) {
    switch (phase) {
        case MapPhase::Warmup: return QStringLiteral("warmup");
        case MapPhase::Live: return QStringLiteral("live");
        case MapPhase::Intermission: return QStringLiteral("intermission");
        case MapPhase::GameOver: return QStringLiteral("gameover");
        default: return QStringLiteral("unknown");
    }
}

/**
 * @brief 炸弹状态枚举（完整状态）
 */
enum class BombState {
    Unknown,
    Carried,
    Dropped,
    Planting,
    Planted,
    Exploded,
    Defusing,
    Defused
};

inline BombState stringToBombState(const QString &str) {
    if (str == "carried") return BombState::Carried;
    if (str == "dropped") return BombState::Dropped;
    if (str == "planting") return BombState::Planting;
    if (str == "planted") return BombState::Planted;
    if (str == "exploded") return BombState::Exploded;
    if (str == "defusing") return BombState::Defusing;
    if (str == "defused") return BombState::Defused;
    return BombState::Unknown;
}

inline QString bombStateToString(BombState state) {
    switch (state) {
        case BombState::Carried: return QStringLiteral("carried");
        case BombState::Dropped: return QStringLiteral("dropped");
        case BombState::Planting: return QStringLiteral("planting");
        case BombState::Planted: return QStringLiteral("planted");
        case BombState::Exploded: return QStringLiteral("exploded");
        case BombState::Defusing: return QStringLiteral("defusing");
        case BombState::Defused: return QStringLiteral("defused");
        default: return QStringLiteral("unknown");
    }
}

/**
 * @brief 稳定炸弹状态枚举（仅关注安放/爆炸/拆除）
 */
enum class StableBombState {
    Unknown,
    Planted,
    Exploded,
    Defused
};

inline StableBombState stringToStableBombState(const QString &str) {
    if (str == "planted") return StableBombState::Planted;
    if (str == "exploded") return StableBombState::Exploded;
    if (str == "defused") return StableBombState::Defused;
    return StableBombState::Unknown;
}

inline QString stableBombStateToString(StableBombState state) {
    switch (state) {
        case StableBombState::Planted: return QStringLiteral("planted");
        case StableBombState::Exploded: return QStringLiteral("exploded");
        case StableBombState::Defused: return QStringLiteral("defused");
        default: return QStringLiteral("unknown");
    }
}

/**
 * @brief 武器状态枚举
 */
enum class WeaponState {
    Unknown,
    Active,
    Holstered,
    Reloading
};

inline WeaponState stringToWeaponState(const QString &str) {
    if (str == "active") return WeaponState::Active;
    if (str == "holstered") return WeaponState::Holstered;
    if (str == "reloading") return WeaponState::Reloading;
    return WeaponState::Unknown;
}

inline QString weaponStateToString(WeaponState state) {
    switch (state) {
        case WeaponState::Active: return QStringLiteral("active");
        case WeaponState::Holstered: return QStringLiteral("holstered");
        case WeaponState::Reloading: return QStringLiteral("reloading");
        default: return QStringLiteral("unknown");
    }
}

/**
 * @brief 武器类型枚举
 */
enum class WeaponType {
    Unknown,
    Pistol,
    Knife,
    SubmachineGun,
    Rifle,
    Shotgun,
    Grenade,
    SniperRifle,
    C4,
    StackableItem,
    Tablet,
    Fists,
    BreachCharge,
    Melee,
    MachineGun
};

inline WeaponType stringToWeaponType(const QString &str) {
    if (str == "Pistol") return WeaponType::Pistol;
    if (str == "Knife") return WeaponType::Knife;
    if (str == "SubmachineGun") return WeaponType::SubmachineGun;
    if (str == "Rifle") return WeaponType::Rifle;
    if (str == "Shotgun") return WeaponType::Shotgun;
    if (str == "Grenade") return WeaponType::Grenade;
    if (str == "SniperRifle") return WeaponType::SniperRifle;
    if (str == "C4") return WeaponType::C4;
    if (str == "StackableItem") return WeaponType::StackableItem;
    if (str == "Tablet") return WeaponType::Tablet;
    if (str == "Fists") return WeaponType::Fists;
    if (str == "BreachCharge") return WeaponType::BreachCharge;
    if (str == "Melee") return WeaponType::Melee;
    if (str == "MachineGun") return WeaponType::MachineGun;
    return WeaponType::Unknown;
}

inline QString weaponTypeToString(WeaponType type) {
    switch (type) {
        case WeaponType::Pistol: return QStringLiteral("Pistol");
        case WeaponType::Knife: return QStringLiteral("Knife");
        case WeaponType::SubmachineGun: return QStringLiteral("SubmachineGun");
        case WeaponType::Rifle: return QStringLiteral("Rifle");
        case WeaponType::Shotgun: return QStringLiteral("Shotgun");
        case WeaponType::Grenade: return QStringLiteral("Grenade");
        case WeaponType::SniperRifle: return QStringLiteral("SniperRifle");
        case WeaponType::C4: return QStringLiteral("C4");
        case WeaponType::StackableItem: return QStringLiteral("StackableItem");
        case WeaponType::Tablet: return QStringLiteral("Tablet");
        case WeaponType::Fists: return QStringLiteral("Fists");
        case WeaponType::BreachCharge: return QStringLiteral("BreachCharge");
        case WeaponType::Melee: return QStringLiteral("Melee");
        case WeaponType::MachineGun: return QStringLiteral("MachineGun");
        default: return QStringLiteral("Unknown");
    }
}

/**
 * @brief 手雷类型枚举
 */
enum class GrenadeType {
    Unknown,
    Smoke,
    Incendiary,
    Molotov,
    HighExplosive,
    Flashbang,
    Decoy
};

inline GrenadeType stringToGrenadeType(const QString &str) {
    if (str == "smoke") return GrenadeType::Smoke;
    if (str == "inferno") return GrenadeType::Incendiary;
    if (str == "firebomb") return GrenadeType::Molotov;
    if (str == "frag") return GrenadeType::HighExplosive;
    if (str == "flashbang") return GrenadeType::Flashbang;
    if (str == "decoy") return GrenadeType::Decoy;
    return GrenadeType::Unknown;
}

inline QString grenadeTypeToString(GrenadeType type) {
    switch (type) {
        case GrenadeType::Smoke: return QStringLiteral("smoke");
        case GrenadeType::Incendiary: return QStringLiteral("inferno");
        case GrenadeType::Molotov: return QStringLiteral("firebomb");
        case GrenadeType::HighExplosive: return QStringLiteral("frag");
        case GrenadeType::Flashbang: return QStringLiteral("flashbang");
        case GrenadeType::Decoy: return QStringLiteral("decoy");
        default: return QStringLiteral("unknown");
    }
}

/**
 * @brief 倒计时阶段枚举
 */
enum class CountdownPhase {
    Unknown,
    Warmup,
    Live,
    Over,
    Freezetime,
    Bomb,
    Defusing,
    Paused,
    Timeout
};

inline CountdownPhase stringToCountdownPhase(const QString &str) {
    if (str == "warmup") return CountdownPhase::Warmup;
    if (str == "live") return CountdownPhase::Live;
    if (str == "over") return CountdownPhase::Over;
    if (str == "freezetime") return CountdownPhase::Freezetime;
    if (str == "bomb") return CountdownPhase::Bomb;
    if (str == "defuse") return CountdownPhase::Defusing;
    if (str == "paused") return CountdownPhase::Paused;
    if (str == "timeout") return CountdownPhase::Timeout;
    return CountdownPhase::Unknown;
}

inline QString countdownPhaseToString(CountdownPhase phase) {
    switch (phase) {
        case CountdownPhase::Warmup: return QStringLiteral("warmup");
        case CountdownPhase::Live: return QStringLiteral("live");
        case CountdownPhase::Over: return QStringLiteral("over");
        case CountdownPhase::Freezetime: return QStringLiteral("freezetime");
        case CountdownPhase::Bomb: return QStringLiteral("bomb");
        case CountdownPhase::Defusing: return QStringLiteral("defuse");
        case CountdownPhase::Paused: return QStringLiteral("paused");
        case CountdownPhase::Timeout: return QStringLiteral("timeout");
        default: return QStringLiteral("unknown");
    }
}

/**
 * @brief 游戏模式枚举
 */
enum class GameMode {
    Unknown,
    Custom,
    Wargames,
    Competitive,
    Wingman,
    WeaponsExpert,
    Casual,
    Cooperative,
    Training,
    Deathmatch,
    Armsrace,
    Retakes,
    TrainingDay,
    Demolition,
    CoopStrike,
    DangerZone,
    Workshop
};

inline GameMode stringToGameMode(const QString &str) {
    if (str == "custom") return GameMode::Custom;
    if (str == "skirmish") return GameMode::Wargames;
    if (str == "competitive") return GameMode::Competitive;
    if (str == "scrimcomp2v2") return GameMode::Wingman;
    if (str == "scrimcomp5v5") return GameMode::WeaponsExpert;
    if (str == "casual") return GameMode::Casual;
    if (str == "cooperative") return GameMode::Cooperative;
    if (str == "training") return GameMode::Training;
    if (str == "deathmatch") return GameMode::Deathmatch;
    if (str == "gungameprogressive") return GameMode::Armsrace;
    if (str == "retakes") return GameMode::Retakes;
    if (str == "new_user_training") return GameMode::TrainingDay;
    if (str == "gungametrbomb") return GameMode::Demolition;
    if (str == "coopmission") return GameMode::CoopStrike;
    if (str == "survival") return GameMode::DangerZone;
    if (str == "workshop") return GameMode::Workshop;
    return GameMode::Unknown;
}

inline QString gameModeToString(GameMode mode) {
    switch (mode) {
        case GameMode::Custom: return QStringLiteral("custom");
        case GameMode::Wargames: return QStringLiteral("skirmish");
        case GameMode::Competitive: return QStringLiteral("competitive");
        case GameMode::Wingman: return QStringLiteral("scrimcomp2v2");
        case GameMode::WeaponsExpert: return QStringLiteral("scrimcomp5v5");
        case GameMode::Casual: return QStringLiteral("casual");
        case GameMode::Cooperative: return QStringLiteral("cooperative");
        case GameMode::Training: return QStringLiteral("training");
        case GameMode::Deathmatch: return QStringLiteral("deathmatch");
        case GameMode::Armsrace: return QStringLiteral("gungameprogressive");
        case GameMode::Retakes: return QStringLiteral("retakes");
        case GameMode::TrainingDay: return QStringLiteral("new_user_training");
        case GameMode::Demolition: return QStringLiteral("gungametrbomb");
        case GameMode::CoopStrike: return QStringLiteral("coopmission");
        case GameMode::DangerZone: return QStringLiteral("survival");
        case GameMode::Workshop: return QStringLiteral("workshop");
        default: return QStringLiteral("unknown");
    }
}

/**
 * @brief 回合胜利条件枚举
 */
enum class RoundWinCondition {
    Unknown,
    CTWinsByElimination,
    CTWinsByDefusal,
    CTWinsByTimeRunout,
    TWinsByElimination,
    TWinsByExplosion
};

inline RoundWinCondition stringToRoundWinCondition(const QString &str) {
    if (str == "ct_win_elimination") return RoundWinCondition::CTWinsByElimination;
    if (str == "ct_win_defuse") return RoundWinCondition::CTWinsByDefusal;
    if (str == "ct_win_time") return RoundWinCondition::CTWinsByTimeRunout;
    if (str == "t_win_elimination") return RoundWinCondition::TWinsByElimination;
    if (str == "t_win_bomb") return RoundWinCondition::TWinsByExplosion;
    return RoundWinCondition::Unknown;
}

inline QString roundWinConditionToString(RoundWinCondition condition) {
    switch (condition) {
        case RoundWinCondition::CTWinsByElimination: return QStringLiteral("ct_win_elimination");
        case RoundWinCondition::CTWinsByDefusal: return QStringLiteral("ct_win_defuse");
        case RoundWinCondition::CTWinsByTimeRunout: return QStringLiteral("ct_win_time");
        case RoundWinCondition::TWinsByElimination: return QStringLiteral("t_win_elimination");
        case RoundWinCondition::TWinsByExplosion: return QStringLiteral("t_win_bomb");
        default: return QStringLiteral("unknown");
    }
}

/**
 * @brief 验证字符串是否为有效的 SteamID64
 * @param str 待验证的字符串
 * @return true 表示是有效的 17 位数字 SteamID64
 */
inline bool isSteamId64(const QString &str) {
    return str.length() == 17 && str.contains(QRegularExpression(QStringLiteral("^[0-9]+$")));
}

} // namespace GSI

#endif // GSIENUMS_H
