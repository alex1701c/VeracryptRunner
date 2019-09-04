#include "VeracryptConfigItem.h"

/*
 * TODO File Pickers
 * TODO Connect signals
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
}

void VeracryptConfigItem::toggleVolumeSource() {
    bool fileSource = this->fileRadioButton->isChecked();
    this->filePushButton->setHidden(!fileSource);
    this->devicePushButton->setHidden(fileSource);
}

void VeracryptConfigItem::initializeValues() {
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
}
