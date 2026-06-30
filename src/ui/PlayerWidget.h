#ifndef PLAYERWIDGET_H
#define PLAYERWIDGET_H

#include <QWidget>

QT_BEGIN_NAMESPACE
namespace Ui { class PlayerWidget; }
QT_END_NAMESPACE

namespace GSI { class GsiService; }

/**
 * @brief 玩家信息面板，显示 HP、护甲、武器、KDA 等
 */
class PlayerWidget : public QWidget {
    Q_OBJECT
public:
    explicit PlayerWidget(QWidget *parent = nullptr);
    ~PlayerWidget() override;
    void connectToService(GSI::GsiService *service);

private:
    Ui::PlayerWidget *ui;
};

#endif // PLAYERWIDGET_H
