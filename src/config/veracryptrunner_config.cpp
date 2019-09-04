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

VeracryptConfigItem::VeracryptConfigItem(QWidget *parent) : QWidget(parent) {
    setupUi(this);
    this->keyFileListView->setHidden(true);
    toggleVolumeSource();

    // Connect Signals
    connect(this->fileRadioButton, SIGNAL(clicked(bool)), this, SLOT(toggleVolumeSource()));
    connect(this->deviceRadioButton, SIGNAL(clicked(bool)), this, SLOT(toggleVolumeSource()));
}

void VeracryptConfigItem::toggleVolumeSource() {
    bool fileSource = this->fileRadioButton->isChecked();
    this->filePushButton->setHidden(!fileSource);
    this->devicePushButton->setHidden(fileSource);
}

VeracryptRunnerConfig::VeracryptRunnerConfig(QWidget *parent, const QVariantList &args) : KCModule(parent, args) {
    m_ui = new VeracryptRunnerConfigForm(this);
    auto *layout = new QGridLayout(this);
    layout->addWidget(m_ui, 0, 0);

    connect(m_ui->pushButton, SIGNAL(clicked(bool)), this, SLOT(changed()));
    connect(m_ui->pushButton, SIGNAL(clicked(bool)), this, SLOT(addVeracryptItem()));
}

void VeracryptRunnerConfig::load() {
    KCModule::load();
    emit changed(false);
}


void VeracryptRunnerConfig::save() {

    KCModule::save();

    emit changed();
}

void VeracryptRunnerConfig::defaults() {
    emit changed(true);
}

void VeracryptRunnerConfig::addVeracryptItem(const VeracryptVolume &volume) {
    Q_UNUSED(volume)
    auto *item = new VeracryptConfigItem(this);
    m_ui->veracryptVolumes->addWidget(item);
}


#include "veracryptrunner_config.moc"

