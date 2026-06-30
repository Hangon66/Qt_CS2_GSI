#ifndef MAPWIDGET_H
#define MAPWIDGET_H

#include <QWidget>
#include <QTimer>
#include "GsiEnums.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MapWidget; }
QT_END_NAMESPACE

namespace GSI { class GsiService; }

/**
 * @brief 地图信息面板，显示地图名、比分、回合数、倒计时等
 */
class MapWidget : public QWidget {
    Q_OBJECT
public:
    explicit MapWidget(QWidget *parent = nullptr);
    ~MapWidget() override;
    void connectToService(GSI::GsiService *service);

private:
    /**
     * @brief 估算倒计时类型
     */
    enum class EstimateType { None, Freezetime, Round, Bomb };

    /** @brief 更新倒计时标签（观战精确模式） */
    void updateCountdownLabel();
    /** @brief 启动估算倒计时 */
    void startEstimatedCountdown(EstimateType type, int seconds);
    /** @brief 停止估算倒计时 */
    void stopEstimatedCountdown();
    /** @brief 估算倒计时每秒递减 */
    void onEstimatedTick();

    Ui::MapWidget *ui;
    /** @brief 观战模式倒计时阶段 */
    GSI::CountdownPhase m_countdownPhase = GSI::CountdownPhase::Unknown;
    /** @brief 观战模式倒计时时间字符串 */
    QString m_countdownTime;
    /** @brief 估算倒计时定时器 */
    QTimer *m_countdownTimer;
    /** @brief 当前估算倒计时类型 */
    EstimateType m_estimateType = EstimateType::None;
    /** @brief 估算剩余秒数 */
    int m_estimatedSeconds = 0;
    /** @brief 是否处于观战模式（phase_countdowns 有数据） */
    bool m_spectatorMode = false;
};

#endif // MAPWIDGET_H
