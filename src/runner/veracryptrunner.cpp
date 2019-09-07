#include "veracryptrunner.h"
#include "VolumeCommandBuilder.h"
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
    QList<Plasma::QueryMatch> matches;

    for (const auto &volume:volumes) {
        if (volume.name.startsWith(volumeQuery, Qt::CaseInsensitive)) {
            matches.append(createMatch(volume));
        }
    }

    context.addMatches(matches);
}

Plasma::QueryMatch VeracryptRunner::createMatch(const VeracryptVolume &volume) {
    Plasma::QueryMatch match(this);
    match.setText(volume.name);
    // match.setText(volume.name + " " + QString::number(match.relevance()));
    match.setData(volume.id);
    match.setIconName("veracrypt");
    match.setRelevance((float) volume.priority / 100);
    return match;
}

void VeracryptRunner::run(const Plasma::RunnerContext &context, const Plasma::QueryMatch &match) {
    Q_UNUSED(context)
    const int id = match.data().toInt();
    VeracryptVolume volume{};

    for (const auto &v:volumes) {
        if (v.id == id) {
            volume = v;
            break;
        }
    }
    VolumeCommandBuilder::build(volume);
}

K_EXPORT_PLASMA_RUNNER(veracryptrunner, VeracryptRunner)

// needed for the QObject subclass declared as part of K_EXPORT_PLASMA_RUNNER
#include "veracryptrunner.moc"
