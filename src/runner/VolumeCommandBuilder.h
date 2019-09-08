#ifndef VERACRYPTRUNNER_VOLUMECOMMANDBUILDER_H
#define VERACRYPTRUNNER_VOLUMECOMMANDBUILDER_H

#include <QtCore/QProcess>
#include <VeracryptVolume.h>

class VolumeCommandBuilder {
public:
    static void buildUnmountCommand(const VeracryptVolume &volume) {
        QProcess::startDetached("veracrypt", QStringList() << "-d" << volume.source);
    }

    static void buildMountCommand(const VeracryptVolume &volume) {
        QStringList options;
        options.append("--keyfiles");
        options.append(volume.keyFiles.join(","));
        options.append(volume.source);
        options.append(volume.mountPath);

        QProcess::startDetached("veracrypt", options);

        // Optional pass integration
        if (!volume.passPath.isEmpty()) {
            QProcess passProcess;
            passProcess.start("pass", QStringList() << "show" << volume.passPath);
            passProcess.waitForFinished(-1);
            const QString passResults = passProcess.readAll();
            if (!passResults.isEmpty()) {
                QString password = passResults.split('\n', QString::SkipEmptyParts).at(0);
                if (!password.isEmpty()) {
                    QProcess windowIdProcess;
                    QStringList idOptions({"-name", "Enter password for \"" + volume.source + "\""});
                    windowIdProcess.start("xwininfo", idOptions);
                    windowIdProcess.waitForFinished(-1);
                    QString windowIdRes = windowIdProcess.readAll();
                    if (windowIdRes.contains("Window id: ")) {
                        const auto id = windowIdRes.split("Window id: ").at(1).split(" ").at(0);
                        QProcess::startDetached("xdotool", QStringList() << "type" << "--window" << id << password);
                    }
                }
            }
        }
    }
};

#endif //VERACRYPTRUNNER_VOLUMECOMMANDBUILDER_H
