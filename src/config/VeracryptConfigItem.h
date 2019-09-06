#include "ui_veracryptrunner_config_item.h"
#include <KConfigCore/KConfigGroup>
#include <VeracryptVolume.h>

#ifndef VERACRYPTRUNNER_CONFIG_ITEM_H
#define VERACRYPTRUNNER_CONFIG_ITEM_H

class VeracryptConfigItem : public QWidget, public Ui::VeracryptItemUi {
Q_OBJECT

public:
    VeracryptVolume *volume;

    explicit VeracryptConfigItem(QWidget *parent, VeracryptVolume *volume = new VeracryptVolume());

    void initializeValues();

public Q_SLOTS:

    void toggleVolumeSource();

    void openVolumeFilePicker();

    // TODO Implement better solution
    void openVolumeDevicePicker();

    void openMountPathPicker();

    void validateKeyFileControls();

    void openKeyFilePicker();

    void removeKeyFile();

    void deleteConfig();

    void passFilePicker();

public:
Q_SIGNALS:

    void confirmedDelete();
};

#endif
