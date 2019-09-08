#ifndef VERACRYPTRUNNER_H
#define VERACRYPTRUNNER_H

#include <KRunner/AbstractRunner>
#include <VeracryptVolume.h>
#include <VeracryptVolumeManager.h>

class VeracryptRunner : public Plasma::AbstractRunner {
Q_OBJECT

public:
    VeracryptRunner(QObject *parent, const QVariantList &args);

    ~VeracryptRunner() override;

    Plasma::QueryMatch createMatch(const VeracryptVolume &volume, bool unmount = false);

    VeracryptVolumeManager manager;
    QList<VeracryptVolume> volumes;
    QStringList mountedVolumes;

public: // Plasma::AbstractRunner API
    void match(Plasma::RunnerContext &context) override;

    void run(const Plasma::RunnerContext &context, const Plasma::QueryMatch &match) override;

    void reloadConfiguration() override;

public Q_SLOTS:

    void init() override;

    void prepareForMatchSession();
};

#endif
