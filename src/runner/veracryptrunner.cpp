#include "veracryptrunner.h"
#include "VolumeCommandBuilder.h"
#include <KLocalizedString>
#include <QtGui/QtGui>

VeracryptRunner::VeracryptRunner(QObject *parent, const QVariantList &args)
        : Plasma::AbstractRunner(parent, args) {
    setObjectName(QStringLiteral("VeracryptRunner"));
}

VeracryptRunner::~VeracryptRunner() = default;

void VeracryptRunner::init() {
    connect(this, SIGNAL(prepare()), this, SLOT(prepareForMatchSession()));
    reloadConfiguration();
}

void VeracryptRunner::prepareForMatchSession() {
    mountedVolumes.clear();
    QProcess fetchVolumesProcess;
    fetchVolumesProcess.start("veracrypt", QStringList() << "-t" << "-l");
    fetchVolumesProcess.waitForFinished(-1);
    const QString res = fetchVolumesProcess.readAll();
    if (!res.isEmpty()) {
        for (const auto &mountedVolume:res.split("\n")) {
            QRegExp pathRegex(R"(\d*: ([^ ]+))");
            pathRegex.indexIn(mountedVolume);
            const auto path = pathRegex.capturedTexts().at(1);
            if (!path.isEmpty()) mountedVolumes.append(path);
        }
    }
}

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
            matches.append(createMatch(volume, mountedVolumes.contains(volume.source)));
        }
    }

    context.addMatches(matches);
}

Plasma::QueryMatch VeracryptRunner::createMatch(const VeracryptVolume &volume, bool unmount) {
    Plasma::QueryMatch match(this);
    if (unmount) {
        match.setText("Unmount " + volume.name);
        match.setData(QMap<QString, QVariant>({
                                                      {"action", "unmount"},
                                                      {"id",     volume.id}}));
        match.setRelevance(0);
    } else {
        match.setText(volume.name);
        match.setData(QMap<QString, QVariant>({
                                                      {"action", "mount"},
                                                      {"id",     volume.id}}));
        match.setRelevance((float) volume.priority / 100);
    }
    //match.setText(match.text() + " " + QString::number(match.relevance()));
    match.setIconName("veracrypt");
    return match;
}

void VeracryptRunner::run(const Plasma::RunnerContext &context, const Plasma::QueryMatch &match) {
    Q_UNUSED(context)

    const QMap<QString, QVariant> data = match.data().toMap();
    const int id = data.value("id", "0").toInt();
    VeracryptVolume volume{};

    for (const auto &v:volumes) {
        if (v.id == id) {
            volume = v;
            break;
        }
    }
    if (data.value("action") == "mount") {
        VolumeCommandBuilder::buildMountCommand(volume);
    } else {
        VolumeCommandBuilder::buildUnmountCommand(volume);
    }
}

K_EXPORT_PLASMA_RUNNER(veracryptrunner, VeracryptRunner)

// needed for the QObject subclass declared as part of K_EXPORT_PLASMA_RUNNER
#include "veracryptrunner.moc"
