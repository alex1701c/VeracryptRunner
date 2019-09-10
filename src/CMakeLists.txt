add_definitions(-DTRANSLATION_DOMAIN=\"plasma_runner_org.kde.veracryptrunner\")

set(veracryptrunner_SRCS runner/veracryptrunner.cpp VeracryptVolume.h VeracryptVolumeManager.h runner/VolumeCommandBuilder.h)

add_library(krunner_veracryptrunner MODULE ${veracryptrunner_SRCS})
target_link_libraries(krunner_veracryptrunner KF5::I18n KF5::ConfigWidgets KF5::Runner)

set(kcm_krunner_veracryptrunner_SRCS
        config/veracryptrunner_config.cpp config/VeracryptConfigItem.cpp config/VeracryptConfigItem.h
        VeracryptVolume.h VeracryptVolumeManager.h)

ki18n_wrap_ui(kcm_krunner_veracryptrunner_SRCS config/veracryptrunner_config.ui)
ki18n_wrap_ui(kcm_krunner_veracryptrunner_SRCS config/veracryptrunner_config_item.ui)
add_library(kcm_krunner_veracryptrunner MODULE ${kcm_krunner_veracryptrunner_SRCS})
target_link_libraries(kcm_krunner_veracryptrunner
        Qt5::Core
        Qt5::Gui
        KF5::CoreAddons
        KF5::ConfigCore
        KF5::I18n
        KF5::ConfigWidgets
        KF5::Runner
        )

add_dependencies(krunner_veracryptrunner kcm_krunner_veracryptrunner)


install(TARGETS krunner_veracryptrunner kcm_krunner_veracryptrunner DESTINATION ${PLUGIN_INSTALL_DIR})
install(FILES runner/plasma-runner-veracryptrunner.desktop config/plasma-runner-veracryptrunner_config.desktop DESTINATION ${KDE_INSTALL_KSERVICES5DIR})
