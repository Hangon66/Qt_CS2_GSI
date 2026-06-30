#ifndef MAPWIDGET_H
#define MAPWIDGET_H

#include <QWidget>

QT_BEGIN_NAMESPACE
namespace Ui { class MapWidget; }
QT_END_NAMESPACE

namespace GSI { class GsiService; }

/**
 * @brief 地图信息面板，显示地图名、比分、回合数等
 */
class MapWidget : public QWidget {
    Q_OBJECT
public:
    explicit MapWidget(QWidget *parent = nullptr);
    ~MapWidget() override;
    void connectToService(GSI::GsiService *service);

private:
    Ui::MapWidget *ui;
};

#endif // MAPWIDGET_H
