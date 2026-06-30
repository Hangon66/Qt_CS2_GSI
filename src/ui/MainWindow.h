#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

namespace GSI { class GsiService; }
class MapWidget;
class PlayerWidget;
class EventLogWidget;

/**
 * @brief 主窗口，整合地图、玩家、事件日志三个面板
 * @details 持有 GsiService 实例并将其连接到各子面板
 */
class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow() override;

    /** @brief 设置 GSI 服务实例并连接各面板 */
    void setService(GSI::GsiService *service);

private:
    Ui::MainWindow *ui;
    MapWidget *m_mapWidget = nullptr;
    PlayerWidget *m_playerWidget = nullptr;
    EventLogWidget *m_eventLogWidget = nullptr;
    GSI::GsiService *m_service = nullptr;
};

#endif // MAINWINDOW_H
