#ifndef VERACRYPTRUNNER_VERACRYPTVOLUME_H
#define VERACRYPTRUNNER_VERACRYPTVOLUME_H

#include <QtCore/QString>
#include <QtCore/QStringList>

enum VolumeType {
    File,
    Device
};

class VeracryptVolume {

public :

    VolumeType type;
    QString source;
    QString mountPath;
    QStringList keyFiles;
    QString passPath;


};

#endif //VERACRYPTRUNNER_VERACRYPTVOLUME_H
