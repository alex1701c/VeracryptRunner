#include <QtWidgets/QFileDialog>
#include <QDebug>
#include "VeracryptConfigItem.h"

/*
 * TODO Save Config
 * TODO Load Config
 */
VeracryptConfigItem::VeracryptConfigItem(QWidget *parent, VeracryptVolume *volume) : QWidget(parent) {
    setupUi(this);
    this->volume = volume;
    initializeValues();

    // Connect type signals
    connect(this->fileRadioButton, SIGNAL(clicked(bool)), this, SLOT(toggleVolumeSource()));
    connect(this->deviceRadioButton, SIGNAL(clicked(bool)), this, SLOT(toggleVolumeSource()));
    connect(this->fileRadioButton, SIGNAL(clicked(bool)), parent, SLOT(changed()));
    connect(this->deviceRadioButton, SIGNAL(clicked(bool)), parent, SLOT(changed()));
    // Connect file/device/path pickers
    connect(this->filePushButton, SIGNAL(clicked(bool)), this, SLOT(openVolumeFilePicker()));
    connect(this->filePushButton, SIGNAL(clicked(bool)), parent, SLOT(changed()));
    connect(this->devicePushButton, SIGNAL(clicked(bool)), this, SLOT(openVolumeDevicePicker()));
    connect(this->devicePushButton, SIGNAL(clicked(bool)), parent, SLOT(changed()));
    connect(this->mountPath, SIGNAL(clicked(bool)), this, SLOT(openMountPathPicker()));
    connect(this->mountPath, SIGNAL(clicked(bool)), parent, SLOT(changed()));
    // Key file controls
    connect(this->keyFileListWidget, SIGNAL(currentRowChanged(int)), this, SLOT(validateKeyFileControls()));
    connect(this->addKeyFileButton, SIGNAL(clicked(bool)), this, SLOT(openKeyFilePicker()));
    connect(this->addKeyFileButton, SIGNAL(clicked(bool)), parent, SLOT(changed()));
    connect(this->removeKeyFileButton, SIGNAL(clicked(bool)), this, SLOT(removeKeyFile()));
    connect(this->removeKeyFileButton, SIGNAL(clicked(bool)), parent, SLOT(changed()));
}

void VeracryptConfigItem::toggleVolumeSource() {
    bool fileSource = this->fileRadioButton->isChecked();
    this->filePushButton->setHidden(!fileSource);
    this->devicePushButton->setHidden(fileSource);
}

void VeracryptConfigItem::initializeValues() {
    this->nameLineEdit->setText(volume->name.isEmpty() ? "New Volume " : volume->name);
    // Initialize type and source
    bool fileType = this->volume->type == "FILE";
    if (fileType) {
        this->filePushButton->setText(volume->source.isEmpty() ? "Select File" : volume->source);
    } else {
        this->devicePushButton->setText(volume->source.isEmpty() ? "Select Device" : volume->source);
    }
    this->filePushButton->setChecked(fileType);
    this->devicePushButton->setChecked(!fileType);
    toggleVolumeSource();

    // Initialize Key Files
    for (const auto &keyFile:volume->keyFiles) {
        this->keyFileListWidget->addItem(keyFile);
    }
    validateKeyFileControls();
}

void VeracryptConfigItem::openVolumeFilePicker() {
    QString volumePath = QFileDialog::getOpenFileName(this, tr("Select Volume"), QDir::homePath(), tr("Veracrypt Volumes (*.hc)"));
    if (!volumePath.isEmpty()) this->filePushButton->setText(volumePath);
}

void VeracryptConfigItem::openVolumeDevicePicker() {
    QString devicePath = QFileDialog::getOpenFileName(this, tr("Select Device"), "/dev", tr("Veracrypt Devices (sd*)"));
    if (!devicePath.isEmpty()) this->devicePushButton->setText(devicePath);
}

void VeracryptConfigItem::openMountPathPicker() {
    QString mountPath = QFileDialog::getExistingDirectory(this, tr("Select Mount Directory"),
                                                          QDir::homePath(), QFileDialog::ShowDirsOnly);
    if (!mountPath.isEmpty()) this->mountPath->setText(mountPath);
}

void VeracryptConfigItem::openKeyFilePicker() {
    QStringList keyFilePaths = QFileDialog::getOpenFileNames(this, tr("Select Key File"), QDir::homePath(), tr("Key Files (*)"));
    if (!keyFilePaths.isEmpty()) this->keyFileListWidget->addItems(keyFilePaths);
    validateKeyFileControls();
}

void VeracryptConfigItem::removeKeyFile() {
    const int currentIndex = this->keyFileListWidget->currentIndex().row();
    if (currentIndex == -1) return;
    this->keyFileListWidget->takeItem(currentIndex);
    validateKeyFileControls();
}

void VeracryptConfigItem::validateKeyFileControls() {
    const bool empty = this->keyFileListWidget->model()->rowCount() == 0;
    this->keyFileListWidget->setHidden(empty);
    this->removeKeyFileButton->setDisabled(empty || this->keyFileListWidget->currentIndex().row() == -1);
}

