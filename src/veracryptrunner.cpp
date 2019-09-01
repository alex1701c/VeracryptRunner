#include "veracryptrunner.h"
#include <KLocalizedString>
#include <QtGui/QtGui>

VeracryptRunner::VeracryptRunner(QObject *parent, const QVariantList &args)
        : Plasma::AbstractRunner(parent, args) {
    setObjectName(QStringLiteral("VeracryptRunner"));
}

VeracryptRunner::~VeracryptRunner() {
}


void VeracryptRunner::match(Plasma::RunnerContext &context) {
    const QString term = context.query();
    QList<Plasma::QueryMatch> matches;
    Plasma::QueryMatch match(this);
    match.setIconName("kdeapp");
    match.setText("Hello World!");
    matches.append(match);
    context.addMatches(matches);
}

void VeracryptRunner::run(const Plasma::RunnerContext &context, const Plasma::QueryMatch &match) {
    Q_UNUSED(context)
    Q_UNUSED(match)

    // TODO
}

K_EXPORT_PLASMA_RUNNER(veracryptrunner, VeracryptRunner)

// needed for the QObject subclass declared as part of K_EXPORT_PLASMA_RUNNER
#include "veracryptrunner.moc"
