#include "veracryptrunner.h"
#include <KLocalizedString>
#include <QtGui/QtGui>

VeracryptRunner::VeracryptRunner(QObject *parent, const QVariantList &args)
        : Plasma::AbstractRunner(parent, args) {
    setObjectName(QStringLiteral("VeracryptRunner"));
}

VeracryptRunner::~VeracryptRunner() = default;


void VeracryptRunner::reloadConfiguration() {
    volumes = manager.getVeracryptVolumes();
}

void VeracryptRunner::match(Plasma::RunnerContext &context) {
    if (!context.isValid()) return;
    const QString term = context.query();
    if (!term.startsWith("veracryp")) return;

    QRegExp regExp(R"(veracrypt?(?: (.*))?)");
    regExp.indexIn(term);
    const QString volumeQuery = regExp.capturedTexts().at(1);
    qInfo() << volumeQuery;
    QList<Plasma::QueryMatch> matches;

    // TODO

    context.addMatches(matches);
}

Plasma::QueryMatch VeracryptRunner::createMatch(const VeracryptVolume &volume, const QString &query) {
    Q_UNUSED(volume)
    Q_UNUSED(query)
    return Plasma::QueryMatch();
}

void VeracryptRunner::run(const Plasma::RunnerContext &context, const Plasma::QueryMatch &match) {
    Q_UNUSED(context)
    Q_UNUSED(match)

    // TODO
}

K_EXPORT_PLASMA_RUNNER(veracryptrunner, VeracryptRunner)

// needed for the QObject subclass declared as part of K_EXPORT_PLASMA_RUNNER
#include "veracryptrunner.moc"
