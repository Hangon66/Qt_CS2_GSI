#ifndef WEAPONDATA_H
#define WEAPONDATA_H

#include <QString>
#include <QMap>

namespace GSI {

/**
 * @brief 武器静态信息
 */
struct WeaponInfo {
    QString type;
    QString displayName;
};

/**
 * @brief 武器静态数据库，提供武器名称到显示名和类型的映射
 */
class WeaponData {
public:
    /**
     * @brief 根据武器名称查找武器信息
     * @param name 武器内部名称（如 "weapon_ak47"）
     * @return 武器信息，未找到时返回空 WeaponInfo
     */
    static WeaponInfo lookup(const QString &name) {
        static const QMap<QString, WeaponInfo> s_data = buildData();
        return s_data.value(name);
    }

private:
    static QMap<QString, WeaponInfo> buildData() {
        QMap<QString, WeaponInfo> m;
        // Pistols
        m.insert("weapon_deagle",         {"Pistol", "Desert Eagle"});
        m.insert("weapon_revolver",       {"Pistol", "R8 Revolver"});
        m.insert("weapon_glock",          {"Pistol", "Glock-18"});
        m.insert("weapon_usp_silencer",   {"Pistol", "USP-S"});
        m.insert("weapon_cz75a",          {"Pistol", "CZ75-Auto"});
        m.insert("weapon_fiveseven",      {"Pistol", "Five-SeveN"});
        m.insert("weapon_p250",           {"Pistol", "P250"});
        m.insert("weapon_tec9",           {"Pistol", "Tec-9"});
        m.insert("weapon_elite",          {"Pistol", "Dual Berettas"});
        m.insert("weapon_hkp2000",        {"Pistol", "P2000"});

        // Submachine Guns
        m.insert("weapon_mp9",            {"SubmachineGun", "MP9"});
        m.insert("weapon_mac10",          {"SubmachineGun", "MAC-10"});
        m.insert("weapon_bizon",          {"SubmachineGun", "PP-Bizon"});
        m.insert("weapon_mp7",            {"SubmachineGun", "MP7"});
        m.insert("weapon_ump45",          {"SubmachineGun", "UMP-45"});
        m.insert("weapon_p90",            {"SubmachineGun", "P90"});
        m.insert("weapon_mp5sd",          {"SubmachineGun", "MP5-SD"});

        // Rifles
        m.insert("weapon_famas",          {"Rifle", "FAMAS"});
        m.insert("weapon_galilar",        {"Rifle", "Galil AR"});
        m.insert("weapon_m4a4",           {"Rifle", "M4A4"});
        m.insert("weapon_m4a1_silencer",  {"Rifle", "M4A1-S"});
        m.insert("weapon_ak47",           {"Rifle", "AK-47"});
        m.insert("weapon_aug",            {"Rifle", "AUG"});
        m.insert("weapon_sg553",          {"Rifle", "SG-553"});

        // Sniper Rifles
        m.insert("weapon_ssg08",          {"SniperRifle", "SSG 08"});
        m.insert("weapon_awp",            {"SniperRifle", "AWP"});
        m.insert("weapon_scar20",         {"SniperRifle", "SCAR-20"});
        m.insert("weapon_g3sg1",          {"SniperRifle", "G3SG1"});

        // Shotguns
        m.insert("weapon_nova",           {"Shotgun", "Nova"});
        m.insert("weapon_xm1014",         {"Shotgun", "XM1014"});
        m.insert("weapon_mag7",           {"Shotgun", "MAG-7"});
        m.insert("weapon_sawedoff",       {"Shotgun", "Sawed-Off"});

        // Machine Guns
        m.insert("weapon_m249",           {"MachineGun", "M249"});
        m.insert("weapon_negev",          {"MachineGun", "Negev"});

        // Knives
        m.insert("weapon_bayonet",               {"Knife", "Bayonet"});
        m.insert("weapon_m9_bayonet",            {"Knife", "M9 Bayonet"});
        m.insert("weapon_butterfly",             {"Knife", "Butterfly Knife"});
        m.insert("weapon_falchion",              {"Knife", "Falchion Knife"});
        m.insert("weapon_flip",                  {"Knife", "Flip Knife"});
        m.insert("weapon_gut",                   {"Knife", "Gut Knife"});
        m.insert("weapon_tactical",              {"Knife", "Huntsman Knife"});
        m.insert("weapon_karambit",              {"Knife", "Karambit"});
        m.insert("weapon_survival_bowie",        {"Knife", "Bowie Knife"});
        m.insert("weapon_knife_push",            {"Knife", "Shadow Daggers"});
        m.insert("weapon_knife_t",               {"Knife", "Terrorist Knife"});
        m.insert("weapon_knife_ct",              {"Knife", "CT Knife"});
        m.insert("weapon_knifegg",               {"Knife", "Gold Knife"});
        m.insert("weapon_knife_ursus",           {"Knife", "Ursus Knife"});
        m.insert("weapon_knife_gypsy_jackknife", {"Knife", "Navaja Knife"});
        m.insert("weapon_knife_stiletto",        {"Knife", "Stiletto Knife"});
        m.insert("weapon_knife_widowmaker",      {"Knife", "Talon Knife"});
        m.insert("weapon_knife_canis",           {"Knife", "Survival Knife"});
        m.insert("weapon_knife_cord",            {"Knife", "Paracord Knife"});
        m.insert("weapon_knife_skeleton",        {"Knife", "Skeleton Knife"});
        m.insert("weapon_knife_outdoor",         {"Knife", "Nomad Knife"});
        m.insert("weapon_knife_kukri",           {"Knife", "Kukri Knife"});

        // Grenades
        m.insert("weapon_decoy",          {"Grenade", "Decoy"});
        m.insert("weapon_flashbang",      {"Grenade", "Flashbang"});
        m.insert("weapon_smokegrenade",   {"Grenade", "Smoke Grenade"});
        m.insert("weapon_hegrenade",      {"Grenade", "HE Grenade"});
        m.insert("weapon_molotov",        {"Grenade", "Molotov"});
        m.insert("weapon_incgrenade",     {"Grenade", "Incendiary (CT Molotov)"});

        // C4
        m.insert("weapon_c4",             {"C4", "C4 Explosive"});

        return m;
    }
};

} // namespace GSI

#endif // WEAPONDATA_H
