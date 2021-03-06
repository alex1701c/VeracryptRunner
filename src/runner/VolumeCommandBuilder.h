#ifndef VERACRYPTRUNNER_VOLUMECOMMANDBUILDER_H
#define VERACRYPTRUNNER_VOLUMECOMMANDBUILDER_H

#include <QtCore/QProcess>
#include <VeracryptVolume.h>
#include <QtCore/QThread>
#include <QtWidgets/QMessageBox>
#include <QtCore/QFile>

class VolumeCommandBuilder {
public:
    static void buildUnmountCommand(const VeracryptVolume &volume) {
        QProcess::startDetached("veracrypt", QStringList() << "-d" << volume.source);
    }

    static void buildMountCommand(const VeracryptVolume &volume) {
        QStringList options;

        // Validate Key Files
        for (const auto &keyFile:volume.keyFiles) {
            if (!QFile::exists(keyFile)) {
                QMessageBox::critical(nullptr, "Error", "The key file ( " + keyFile + " ) does not exist!");
                return;
            }
        }
        options.append("--keyfiles");
        options.append(volume.keyFiles.join(","));

        // Validate source
        if (volume.source == "Select File" || volume.source == "Select Device") {
            QMessageBox::critical(nullptr, "Error", "Please select a valid source for the volume!");
            return;
        } else if (!QFile::exists(volume.source)) {
            QMessageBox::critical(nullptr, "Error", "The volume source ( " + volume.source + " ) does not exist!");
            return;
        }
        options.append(volume.source);

        options.append(volume.mountPath);

        QProcess::startDetached("veracrypt", options);

        // Optional pass integration
        if (!volume.passPath.isEmpty()) {
            QProcess passProcess;
            system("$(sleep 0.5; wmctrl -a \"$(hostname)\" )&"); // Gives automatically focus so you can type
            passProcess.start("pass", QStringList() << "show" << volume.passPath);
            passProcess.waitForFinished(-1);
            const QString passResults = passProcess.readAllStandardOutput();
            const QString passError = passProcess.readAllStandardError();
            if (passError.contains(" is not in the password store.")) {
                // Show above Veracrypt dialog
                auto *msgBox = new QMessageBox;
                msgBox->setParent(nullptr);
                msgBox->setIcon(QMessageBox::Icon::Critical);
                msgBox->setWindowTitle("Error");
                msgBox->setText(passError);
                msgBox->setWindowFlags(Qt::WindowStaysOnTopHint);
                msgBox->show();
                // Focus so you can close the message with Esc/Enter without interacting with the Veracrypt dialog
                system("$(sleep 0.25; wmctrl -a \"Error — krunner\") &");
            }
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
