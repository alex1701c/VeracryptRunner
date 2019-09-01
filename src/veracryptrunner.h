#ifndef VERACRYPTRUNNER_H
#define VERACRYPTRUNNER_H

#include <KRunner/AbstractRunner>

class VeracryptRunner : public Plasma::AbstractRunner
{
    Q_OBJECT

public:
    VeracryptRunner(QObject *parent, const QVariantList &args);
    ~VeracryptRunner() override;

public: // Plasma::AbstractRunner API
    void match(Plasma::RunnerContext &context) override;
    void run(const Plasma::RunnerContext &context, const Plasma::QueryMatch &match) override;
};

#endif
