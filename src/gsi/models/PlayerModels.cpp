#include "PlayerModels.h"
#include "WeaponData.h"

namespace GSI {

// --- PlayerState ---
PlayerState::PlayerState(const QJsonObject &obj)
    : health(obj.value("health").toInt())
    , armor(obj.value("armor").toInt())
    , helmet(obj.value("helmet").toBool())
    , flashed(obj.value("flashed").toInt())
    , smoked(obj.value("smoked").toInt())
    , burning(obj.value("burning").toInt())
    , money(obj.value("money").toInt())
    , roundKills(obj.value("round_kills").toInt())
    , roundHeadshotKills(obj.value("round_killhs").toInt())
    , equipmentValue(obj.value("equip_value").toInt())
{}

QJsonObject PlayerState::toJsonObject() const {
    QJsonObject o;
    o["health"] = health; o["armor"] = armor; o["helmet"] = helmet;
    o["flashed"] = flashed; o["smoked"] = smoked; o["burning"] = burning;
    o["money"] = money; o["round_kills"] = roundKills;
    o["round_killhs"] = roundHeadshotKills; o["equip_value"] = equipmentValue;
    return o;
}

bool PlayerState::operator==(const PlayerState &o) const {
    return health == o.health && armor == o.armor && helmet == o.helmet &&
           flashed == o.flashed && smoked == o.smoked && burning == o.burning &&
           money == o.money && roundKills == o.roundKills &&
           roundHeadshotKills == o.roundHeadshotKills && equipmentValue == o.equipmentValue;
}

// --- PlayerMatchStats ---
PlayerMatchStats::PlayerMatchStats(const QJsonObject &obj)
    : kills(obj.value("kills").toInt())
    , assists(obj.value("assists").toInt())
    , deaths(obj.value("deaths").toInt())
    , mvps(obj.value("mvps").toInt())
    , score(obj.value("score").toInt())
{}

QJsonObject PlayerMatchStats::toJsonObject() const {
    QJsonObject o;
    o["kills"] = kills; o["assists"] = assists; o["deaths"] = deaths;
    o["mvps"] = mvps; o["score"] = score;
    return o;
}

bool PlayerMatchStats::operator==(const PlayerMatchStats &o) const {
    return kills == o.kills && assists == o.assists && deaths == o.deaths &&
           mvps == o.mvps && score == o.score;
}

QString PlayerMatchStats::kda() const {
    if (deaths == 0) return QStringLiteral("\xe2\x88\x9e"); // ∞
    return QString::number(static_cast<double>(kills + assists) / deaths, 'f', 2);
}

// --- Weapon ---
Weapon::Weapon(const QJsonObject &obj)
    : name(obj.value("name").toString())
    , state(stringToWeaponState(obj.value("state").toString()))
    , type(stringToWeaponType(obj.value("type").toString()))
    , ammoClip(obj.value("ammo_clip").toInt(-1))
    , ammoClipMax(obj.value("ammo_clip_max").toInt(-1))
    , ammoReserve(obj.value("ammo_reserve").toInt(-1))
{
    const auto &data = WeaponData::lookup(name);
    displayName = data.displayName.isEmpty() ? name : data.displayName;
    if (type == WeaponType::Unknown && !data.type.isEmpty())
        type = stringToWeaponType(data.type);
}

QJsonObject Weapon::toJsonObject() const {
    QJsonObject o;
    o["name"] = name; o["state"] = weaponStateToString(state);
    o["type"] = weaponTypeToString(type);
    if (ammoClip >= 0) o["ammo_clip"] = ammoClip;
    if (ammoClipMax >= 0) o["ammo_clip_max"] = ammoClipMax;
    if (ammoReserve >= 0) o["ammo_reserve"] = ammoReserve;
    return o;
}

bool Weapon::operator==(const Weapon &o) const {
    return name == o.name && state == o.state && ammoClip == o.ammoClip && ammoReserve == o.ammoReserve;
}

bool Weapon::isPrimary() const {
    return type == WeaponType::Rifle || type == WeaponType::SniperRifle ||
           type == WeaponType::Shotgun || type == WeaponType::SubmachineGun ||
           type == WeaponType::MachineGun;
}

// --- WeaponsCollection ---
WeaponsCollection::WeaponsCollection(const QJsonObject &obj) {
    for (auto it = obj.begin(); it != obj.end(); ++it) {
        list.append(Weapon(it.value().toObject()));
    }
}

QJsonObject WeaponsCollection::toJsonObject() const {
    QJsonObject o;
    for (int i = 0; i < list.size(); ++i)
        o[QStringLiteral("weapon_") + QString::number(i)] = list[i].toJsonObject();
    return o;
}

bool WeaponsCollection::operator==(const WeaponsCollection &o) const {
    return list == o.list;
}

const Weapon *WeaponsCollection::getActive() const {
    for (const auto &w : list)
        if (w.isActive()) return &w;
    return nullptr;
}

QList<Weapon> WeaponsCollection::getGrenades() const {
    QList<Weapon> result;
    for (const auto &w : list)
        if (w.isGrenade()) result.append(w);
    return result;
}

bool WeaponsCollection::hasC4() const {
    for (const auto &w : list)
        if (w.isC4()) return true;
    return false;
}

// --- Player ---
Player::Player(const QJsonObject &obj)
    : steamid(obj.value("steamid").toString())
    , name(obj.value("name").toString())
    , clan(obj.value("clan").toString())
    , xpOverloadLevel(obj.value("xpoverload").toInt())
    , team(stringToTeam(obj.value("team").toString()))
    , activity(stringToActivity(obj.value("activity").toString()))
    , observerSlot(obj.value("observer_slot").toInt(-1))
    , specTarget(obj.value("spectarget").toString())
    , position(Vector3D::fromString(obj.value("position").toString()))
    , forwardDirection(Vector3D::fromString(obj.value("forward").toString()))
    , state(obj.value("state").toObject())
    , matchStats(obj.value("match_stats").toObject())
    , weapons(obj.value("weapons").toObject())
{
    activeWeapon = weapons.getActive();
}

QJsonObject Player::toJsonObject() const {
    QJsonObject o;
    if (!steamid.isEmpty()) o["steamid"] = steamid;
    o["name"] = name; o["clan"] = clan;
    o["team"] = teamToString(team); o["activity"] = activityToString(activity);
    if (observerSlot >= 0) o["observer_slot"] = observerSlot;
    if (!specTarget.isEmpty()) o["spectarget"] = specTarget;
    o["position"] = position.toString(); o["forward"] = forwardDirection.toString();
    o["state"] = state.toJsonObject(); o["match_stats"] = matchStats.toJsonObject();
    o["weapons"] = weapons.toJsonObject();
    return o;
}

bool Player::operator==(const Player &o) const {
    return steamid == o.steamid && name == o.name && team == o.team &&
           activity == o.activity && state == o.state && matchStats == o.matchStats &&
           weapons == o.weapons && position == o.position && forwardDirection == o.forwardDirection;
}

} // namespace GSI
