#ifndef EVENTLOGWIDGET_H
#define EVENTLOGWIDGET_H

#include <QWidget>

QT_BEGIN_NAMESPACE
namespace Ui { class EventLogWidget; }
QT_END_NAMESPACE

namespace GSI { class GsiService; }

/**
 * @brief 事件日志面板，显示 GSI 事件流
 */
class EventLogWidget : public QWidget {
    Q_OBJECT
public:
    explicit EventLogWidget(QWidget *parent = nullptr);
    ~EventLogWidget() override;

    /** @brief 连接到 GsiService 的信号 */
    void connectToService(GSI::GsiService *service);

private slots:
    void onClearClicked();
    void appendLog(const QString &msg);

private:
    Ui::EventLogWidget *ui;
};

#endif // EVENTLOGWIDGET_H
