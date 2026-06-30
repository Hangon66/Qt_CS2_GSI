#ifndef LOCALPLAYERSERVICE_H
#define LOCALPLAYERSERVICE_H

#include <QObject>
#include "models/PlayerModels.h"
#include "GsiEnums.h"

namespace GSI
{

    class GsiService;

    /**
     * @brief 本地玩家数据包装服务
     * @details 内部通过 allPlayers + provider.steamid 自动过滤本地玩家数据，
     *          对外暴露干净的无 steamid 参数信号。
     *          UI 层直接连接这些信号即可，无需关心过滤逻辑。
     *
     * 数据源优先级：
     *   1. allPlayers[provider.steamid]（始终准确，观战安全）
     *   2. player 字段（回退，仅在 player.steamid == provider.steamid 时使用）
     */
    class LocalPlayerService : public QObject
    {
        Q_OBJECT

    public:
        /**
         * @brief 构造本地玩家数据包装服务。
         * @param service GSI 核心服务指针，用于获取游戏状态和连接信号。
         * @param parent Qt 父对象，用于内存管理。
         */
        explicit LocalPlayerService(GsiService *service, QObject *parent = nullptr);

        /**
         * @brief 获取当前本地玩家数据。
         * @return 本地玩家指针，数据不可用时返回 nullptr。
         */
        const Player *localPlayer() const;

        /**
         * @brief 本地玩家数据是否可用。
         * @return true 表示数据可用，false 表示不可用。
         */
        bool isAvailable() const;

    signals:
        // ====== 基础信息 ======
        /**
         * @brief 玩家名称变化。
         * @param previous 旧名称。
         * @param current 新名称。
         */
        void nameChanged(const QString &previous, const QString &current);
        /**
         * @brief 所属队伍变化。
         * @param previous 旧队伍。
         * @param current 新队伍。
         */
        void teamChanged(Team previous, Team current);

        // ====== 实时状态 ======
        /**
         * @brief 生命值变化。
         * @param previous 旧值。
         * @param current 新值。
         */
        void hpChanged(int previous, int current);
        /**
         * @brief 护甲值变化。
         * @param previous 旧值。
         * @param current 新值。
         */
        void armorChanged(int previous, int current);
        /**
         * @brief 头盔状态变化。
         * @param previous 旧值。
         * @param current 新值。
         */
        void helmetChanged(bool previous, bool current);
        /**
         * @brief 金钱变化。
         * @param previous 旧值。
         * @param current 新值。
         */
        void moneyChanged(int previous, int current);
        /**
         * @brief 闪光效果强度变化 (0-255)。
         * @param previous 旧值。
         * @param current 新值。
         */
        void flashedChanged(int previous, int current);
        /**
         * @brief 烟雾效果强度变化 (0-255)。
         * @param previous 旧值。
         * @param current 新值。
         */
        void smokedChanged(int previous, int current);
        /**
         * @brief 燃烧效果强度变化 (0-255)。
         * @param previous 旧值。
         * @param current 新值。
         */
        void burningChanged(int previous, int current);
        /**
         * @brief 拆弹器有无变化。
         * @param previous 旧值。
         * @param current 新值。
         */
        void defusekitChanged(bool previous, bool current);
        /**
         * @brief 本回合击杀数变化。
         * @param previous 旧值。
         * @param current 新值。
         */
        void roundKillsChanged(int previous, int current);
        /**
         * @brief 本回合爆头击杀数变化。
         * @param previous 旧值。
         * @param current 新值。
         */
        void roundHeadshotKillsChanged(int previous, int current);
        /**
         * @brief 本回合总伤害变化。
         * @param previous 旧值。
         * @param current 新值。
         */
        void roundTotalDamageChanged(int previous, int current);

        // ====== 比赛统计 ======
        /**
         * @brief 总击杀数变化。
         * @param previous 旧值。
         * @param current 新值。
         */
        void killsChanged(int previous, int current);
        /**
         * @brief 总死亡数变化。
         * @param previous 旧值。
         * @param current 新值。
         */
        void deathsChanged(int previous, int current);
        /**
         * @brief 总助攻数变化。
         * @param previous 旧值。
         * @param current 新值。
         */
        void assistsChanged(int previous, int current);
        /**
         * @brief 总得分变化。
         * @param previous 旧值。
         * @param current 新值。
         */
        void scoreChanged(int previous, int current);
        /**
         * @brief MVP 次数变化。
         * @param previous 旧值。
         * @param current 新值。
         */
        void mvpsChanged(int previous, int current);

        // ====== 武器 ======
        /**
         * @brief 当前持武器变化。
         * @param previous 旧武器。
         * @param current 新武器。
         */
        void weaponChanged(const Weapon &previous, const Weapon &current);
        /**
         * @brief 弹匣内弹药变化。
         * @param previous 旧值。
         * @param current 新值。
         */
        void ammoClipChanged(int previous, int current);
        /**
         * @brief 备弹量变化。
         * @param previous 旧值。
         * @param current 新值。
         */
        void ammoReserveChanged(int previous, int current);

        // ====== 生命周期 ======
        /**
         * @brief 本地玩家首次出现在数据中。
         * @param player 玩家数据快照。
         */
        void localPlayerJoined(const Player &player);
        /**
         * @brief 本地玩家从数据中消失（断连或数据不可用）。
         * @param player 最后的玩家数据快照。
         */
        void localPlayerLeft(const Player &player);

        // ====== 语义事件（本地玩家） ======
        /**
         * @brief 本地玩家死亡（HP 从 >0 → 0）。
         */
        void died();
        /**
         * @brief 本地玩家重生（HP 从 0 → >0）。
         */
        void respawned();
        /**
         * @brief 本地玩家受到伤害。
         * @param amount 伤害量（HP 减少值，> 0）。
         */
        void tookDamage(int amount);
        /**
         * @brief 本地玩家获得一次击杀（kills +1）。
         * @param weaponName 击杀使用的武器名称。
         */
        void gotKill(const QString &weaponName);
        /**
         * @brief 本地玩家正在安装炸弹。
         */
        void bombPlanting();
        /**
         * @brief 本地玩家安装炸弹完成。
         */
        void bombPlanted();

    private slots:
        /**
         * @brief 响应 GsiService::stateUpdated() 信号，执行本地玩家前后帧对比并发射信号。
         */
        void onStateUpdated();

    private:
        /**
         * @brief 逐字段对比两个 Player 状态并发射对应的变化信号。
         * @param prev 上一帧本地玩家数据。
         * @param curr 当前帧本地玩家数据。
         */
        void compareAndEmit(const Player &prev, const Player &curr);

        /**
         * @brief 对比两个 Player 的武器状态并发射武器相关信号。
         * @param prev 上一帧本地玩家数据。
         * @param curr 当前帧本地玩家数据。
         */
        void compareWeapons(const Player &prev, const Player &curr);

        /**
         * @brief GSI 核心服务指针，用于获取游戏状态。
         */
        GsiService *m_service;

        /**
         * @brief 上一帧本地玩家数据快照，用于前后帧对比。
         */
        Player m_lastLocal;

        /**
         * @brief 上一帧本地玩家数据是否可用，用于检测加入/离开事件。
         */
        bool m_wasAvailable = false;
    };

} // namespace GSI

#endif // LOCALPLAYERSERVICE_H
