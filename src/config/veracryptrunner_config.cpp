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
    auto volumes = manager.getVeracryptVolumes();
    std::sort(volumes.begin(), volumes.end(), [](VeracryptVolume &volume1, VeracryptVolume &volume2) -> bool {
        return volume1.priority < volume2.priority;
    });
    for (auto &volume:volumes) {
        //qInfo() << "Load: " << volume.name << volume.priority;
        addVeracryptItem(&volume, false);
    }
    validateMoveButtons();
    emit changed(false);
}


void VeracryptRunnerConfig::save() {
    qInfo() << "Save Config";
    QList<VeracryptConfigItem *> configItemsUi;
    const int itemCount = m_ui->veracryptVolumes->count();
    for (int i = 0; i < itemCount; ++i) {
        configItemsUi.append(reinterpret_cast<VeracryptConfigItem *>( m_ui->veracryptVolumes->itemAt(i)->widget()));
    }
    manager.saveVeracryptVolumes(configItemsUi);
    emit changed();
}

void VeracryptRunnerConfig::defaults() {
    emit changed(true);
}

void VeracryptRunnerConfig::addVeracryptItem(VeracryptVolume *volume, bool validate) {
    if (volume->id == -1) {
        volume->id = manager.config.group("General").readEntry("id", "1").toInt();
        manager.config.group("General").writeEntry("id", volume->id + 1);
    }
    auto *element = new VeracryptConfigItem(this, volume);
    m_ui->veracryptVolumes->insertWidget(0, element);
    if (validate) validateMoveButtons();
}

void VeracryptRunnerConfig::confirmedDeleteOfItem() {
    auto *item = dynamic_cast<VeracryptConfigItem *>(this->sender());
    m_ui->veracryptVolumes->removeWidget(item);
    item->deleteLater();
}

void VeracryptRunnerConfig::moveItemUp() {
    const auto item = reinterpret_cast<VeracryptConfigItem *>(sender()->parent()->parent());
    const int idx = m_ui->veracryptVolumes->indexOf(item);
    m_ui->veracryptVolumes->removeWidget(item);
    m_ui->veracryptVolumes->insertWidget(idx - 1, item);
    m_ui->scrollArea->ensureWidgetVisible(m_ui->veracryptVolumes->itemAt(idx)->widget());
    validateMoveButtons();
}

void VeracryptRunnerConfig::moveItemDown() {
    const auto item = reinterpret_cast<VeracryptConfigItem *>(sender()->parent()->parent());
    const int idx = m_ui->veracryptVolumes->indexOf(item);
    m_ui->veracryptVolumes->removeWidget(item);
    m_ui->veracryptVolumes->insertWidget(idx + 1, item);
    validateMoveButtons();
    m_ui->scrollArea->ensureWidgetVisible(m_ui->veracryptVolumes->itemAt(idx)->widget());
}

void VeracryptRunnerConfig::validateMoveButtons() {
    const int itemCount = m_ui->veracryptVolumes->count();
    for (int i = 0; i < itemCount; ++i) {
        const auto *item = reinterpret_cast<VeracryptConfigItem *>( m_ui->veracryptVolumes->itemAt(i)->widget());
        item->moveUp->setDisabled(i == 0);
        item->moveDown->setDisabled(i == itemCount - 1);
    }
}


#include "veracryptrunner_config.moc"

