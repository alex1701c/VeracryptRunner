#ifndef VERACRYPTRUNNER_VERACRYPTVOLUMEMANAGER_H
#define VERACRYPTRUNNER_VERACRYPTVOLUMEMANAGER_H

#include <QDebug>
#include <KSharedConfig>
#include "VeracryptVolume.h"

class VeracryptVolumeManager {
public:
    KConfigGroup config;

    VeracryptVolumeManager() {
        this->config = KSharedConfig::openConfig("krunnerrc")->group("Runners").group("VeracryptRunner");
    }

    QList<VeracryptVolume> getVeracryptVolumes() {
        QList<VeracryptVolume> volumes;
        for (const auto &volumeName:config.groupList().filter(QRegExp(R"(^(?!General$).*$)"))) {
            VeracryptVolume volume;
            KConfigGroup volumeConfig = config.group(volumeName);
            volume.name = volumeName;
            volume.id = volumeConfig.readEntry("id").toInt();
            volume.priority = volumeConfig.readEntry("priority").toInt();
            volume.type = volumeConfig.readEntry("type");
            volume.source = volumeConfig.readEntry("source");
            volume.mountPath = volumeConfig.readEntry("mountPath");
            volume.keyFiles = volumeConfig.readEntry("keyFiles").split(";", QString::SplitBehavior::SkipEmptyParts);
            volume.passPath = volumeConfig.readEntry("passPath");
            volumes.append(volume);
        }
        return volumes;
    }

#ifdef CONFIG_DIALOG_MODULE

#include "config/VeracryptConfigItem.h"

    void saveVeracryptVolumes(const QList<VeracryptConfigItem *> &configItems) {
        for (const auto &volumeGroupName:config.groupList().filter(QRegExp(R"(^(?!General$).*$)"))) {
            config.group(volumeGroupName).deleteGroup();
        }
        for (auto *item:configItems) {
            auto group = config.group(item->nameLineEdit->text());
            group.writeEntry("id", item->idLabel->text());
            group.writeEntry("priority", 100 - configItems.indexOf(item));
            //qInfo() << "Save: " << item->nameLineEdit->text() << configItems.indexOf(item);
            group.writeEntry("type", item->fileRadioButton->isChecked() ? "FILE" : "DEVICE");
            group.writeEntry("source",
                             item->fileRadioButton->isChecked() ? item->filePushButton->text() : item->devicePushButton->text()
            );
            group.writeEntry("mountPath", item->mountPath->text());
            QString keyFilesString;
            int count = item->keyFileListWidget->model()->rowCount();
            for (int i = 0; i < count; ++i) keyFilesString.append(item->keyFileListWidget->item(i)->text() + ";");
            group.writeEntry("keyFiles", keyFilesString);
            group.writeEntry("passPath", item->passIntegration->text());
        }
    }

#endif
};

#endif
