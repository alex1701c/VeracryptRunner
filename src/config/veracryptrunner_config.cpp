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
    auto *box = new QGroupBox();
    box->setTitle(!volume.source.isEmpty() ? volume.source : "New Volume");
    auto *itemLayout = new QVBoxLayout(box);

    // Buttons to select the type of volume
    auto *typeRadioButtons = new QGroupBox();
    auto *fileOption = new QRadioButton("File", typeRadioButtons);
    auto *deviceOption = new QRadioButton("Device", typeRadioButtons);
    if(volume.type == "DEVICE") deviceOption->setChecked(true); else fileOption->setChecked(true);
    itemLayout->addWidget(fileOption);
    itemLayout->addWidget(deviceOption);


    m_ui->veracryptVolumes->addWidget(box);
}


#include "veracryptrunner_config.moc"
