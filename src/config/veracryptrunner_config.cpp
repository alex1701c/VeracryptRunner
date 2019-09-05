#include "veracryptrunner_config.h"
#include <KSharedConfig>
#include <KPluginFactory>
#include <krunner/abstractrunner.h>
#include <QtGui>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QRadioButton>


K_PLUGIN_FACTORY(VeracryptRunnerConfigFactory, registerPlugin<VeracryptRunnerConfig>("kcm_krunner_veracryptrunner");)

VeracryptRunnerConfigForm::VeracryptRunnerConfigForm(QWidget *parent) : QWidget(parent) {
    setupUi(this);
}


VeracryptRunnerConfig::VeracryptRunnerConfig(QWidget *parent, const QVariantList &args) : KCModule(parent, args) {
    m_ui = new VeracryptRunnerConfigForm(this);
    auto *layout = new QGridLayout(this);
    layout->addWidget(m_ui, 0, 0);

    connect(m_ui->pushButton, SIGNAL(clicked(bool)), this, SLOT(changed()));
    connect(m_ui->pushButton, SIGNAL(clicked(bool)), this, SLOT(addVeracryptItem()));
}

void VeracryptRunnerConfig::load() {
    const auto volumes = manager.getVeracryptVolumes();
    for (auto &volume:manager.getVeracryptVolumes()) addVeracryptItem(&volume);
    emit changed(false);
}


void VeracryptRunnerConfig::save() {
    qInfo() << "Save Config";
    manager.saveVeracryptVolumes(volumeUiElements);
    emit changed();
}

void VeracryptRunnerConfig::defaults() {
    emit changed(true);
}

void VeracryptRunnerConfig::addVeracryptItem(VeracryptVolume *volume) {
    if (volume->id == -1) {
        volume->id = manager.config.group("General").readEntry("id", "1").toInt();
        manager.config.group("General").writeEntry("id", volume->id + 1);
    }
    auto *element = new VeracryptConfigItem(this, volume);
    m_ui->veracryptVolumes->addWidget(element);
    volumeUiElements.append(element);
}

void VeracryptRunnerConfig::confirmedDeleteOfItem() {
    auto *item = dynamic_cast<VeracryptConfigItem *>(this->sender());
    volumeUiElements.removeAt(volumeUiElements.indexOf(item));
    m_ui->veracryptVolumes->removeWidget(item);
    item->deleteLater();
}


#include "veracryptrunner_config.moc"

