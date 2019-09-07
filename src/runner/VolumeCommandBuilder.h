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
            QString password = QString(passProcess.readAll()).split('\n', QString::SkipEmptyParts).at(0);
            QProcess::startDetached("xdotool", QStringList() << "type" << password);
        }
    }
};

#endif //VERACRYPTRUNNER_VOLUMECOMMANDBUILDER_H
