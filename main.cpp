#include "src/ui/MainWindow.h"
#include "src/gsi/GsiService.h"

#include <QApplication>
#include <QDebug>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    // 创建 GSI 服务
    GSI::GsiService gsiService;

    // 创建主窗口并连接服务
    MainWindow w;
    w.setService(&gsiService);
    w.show();

    // 启动 GSI HTTP 监听（默认端口 3000）
    gsiService.start(3000);
    qDebug() << "[QtGSI] 服务已启动，等待 CS2 推送数据...";

    return QCoreApplication::exec();
}
