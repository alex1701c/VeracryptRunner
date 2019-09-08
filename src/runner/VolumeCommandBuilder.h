#ifndef VERACRYPTRUNNER_VOLUMECOMMANDBUILDER_H
#define VERACRYPTRUNNER_VOLUMECOMMANDBUILDER_H

#include <QtCore/QProcess>
#include <VeracryptVolume.h>

class VolumeCommandBuilder {
public:
    static void build(const VeracryptVolume &volume) {
        QProcess veracryptProcess;
        QStringList options;
        options.append("--keyfiles");
        options.append(volume.keyFiles.join(",").remove("&"));
        options.append(QString(volume.source).remove("&"));
        options.append(QString(volume.mountPath).remove("&"));

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
                    QStringList idOptions({"-name", "Enter password for \"" + QString(volume.source).remove("&") + "\""});
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
