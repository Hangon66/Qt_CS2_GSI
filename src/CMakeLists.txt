set(GSI_SOURCES
    gsi/enums/GsiEnums.h

    gsi/models/Vector3D.h
    gsi/models/Vector3D.cpp
    gsi/models/GsiModels.h
    gsi/models/GsiModels.cpp
    gsi/models/PlayerModels.h
    gsi/models/PlayerModels.cpp
    gsi/models/BombModel.h
    gsi/models/BombModel.cpp
    gsi/models/GrenadeModels.h
    gsi/models/GrenadeModels.cpp
    gsi/models/AllPlayers.h
    gsi/models/AllPlayers.cpp
    gsi/models/GameState.h
    gsi/models/GameState.cpp
    gsi/models/WeaponData.h

    gsi/utils/JsonUtils.h
    gsi/utils/JsonUtils.cpp
    gsi/utils/FileLogger.h
    gsi/utils/FileLogger.cpp
    gsi/network/GsiListener.h
    gsi/network/GsiListener.cpp
    gsi/network/GsiServer.h
    gsi/network/GsiServer.cpp
    gsi/network/GsiConfigWriter.h
    gsi/network/GsiConfigWriter.cpp

    gsi/differs/DifferBase.h
    gsi/differs/DifferManager.h
    gsi/differs/DifferManager.cpp
    gsi/differs/PlayerDiffers.h
    gsi/differs/PlayerDiffers.cpp
    gsi/differs/MapDiffers.h
    gsi/differs/MapDiffers.cpp
    gsi/differs/BombDiffer.h
    gsi/differs/BombDiffer.cpp
    gsi/differs/GrenadesDiffer.h
    gsi/differs/GrenadesDiffer.cpp

    gsi/GsiUpdateHandler.h
    gsi/GsiUpdateHandler.cpp
    gsi/GsiService.h
    gsi/GsiService.cpp
    gsi/LocalPlayerService.h
    gsi/LocalPlayerService.cpp

    ui/MainWindow.h
    ui/MainWindow.cpp
    ui/MainWindow.ui
    ui/PlayerWidget.h
    ui/PlayerWidget.cpp
    ui/PlayerWidget.ui
    ui/MapWidget.h
    ui/MapWidget.cpp
    ui/MapWidget.ui
    ui/EventLogWidget.h
    ui/EventLogWidget.cpp
    ui/EventLogWidget.ui

    utils/SoundPlayer.h
    utils/SoundPlayer.cpp
)

qt_add_executable(QtGSI
    WIN32 MACOSX_BUNDLE
    ${CMAKE_SOURCE_DIR}/main.cpp
    ${GSI_SOURCES}
)

target_include_directories(QtGSI
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}
        ${CMAKE_CURRENT_SOURCE_DIR}/gsi
        ${CMAKE_CURRENT_SOURCE_DIR}/gsi/enums
        ${CMAKE_CURRENT_SOURCE_DIR}/gsi/models
        ${CMAKE_CURRENT_SOURCE_DIR}/gsi/differs
        ${CMAKE_CURRENT_SOURCE_DIR}/gsi/network
        ${CMAKE_CURRENT_SOURCE_DIR}/gsi/utils
        ${CMAKE_CURRENT_SOURCE_DIR}/ui
)

target_link_libraries(QtGSI
    PRIVATE
        Qt::Core
        Qt::Widgets
        Qt::Network
        Qt::WebSockets
        Qt::Multimedia
)

# 拷贝 resources 目录到构建输出目录（与 QtGSI.exe 同级）
add_custom_command(TARGET QtGSI POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_SOURCE_DIR}/resources
        $<TARGET_FILE_DIR:QtGSI>/resources
)

install(TARGETS QtGSI
    BUNDLE  DESTINATION .
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
)

qt_generate_deploy_app_script(
    TARGET QtGSI
    OUTPUT_SCRIPT deploy_script
    NO_UNSUPPORTED_PLATFORM_ERROR
)
install(SCRIPT ${deploy_script})
