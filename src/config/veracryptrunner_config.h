#ifndef VeracryptRunnerCONFIG_H
#define VeracryptRunnerCONFIG_H

#include "ui_veracryptrunner_config.h"
#include <KCModule>
#include <KConfigCore/KConfigGroup>
#include <VeracryptVolume.h>

class VeracryptRunnerConfigForm : public QWidget, public Ui::VeracryptRunnerConfigUi {
Q_OBJECT

public:
    explicit VeracryptRunnerConfigForm(QWidget *parent);
};

class VeracryptRunnerConfig : public KCModule {
Q_OBJECT

public:
    explicit VeracryptRunnerConfig(QWidget *parent = nullptr, const QVariantList &args = QVariantList());

public Q_SLOTS:

    void save() override;

    void load() override;

    void defaults() override;

    void addVeracryptItem(const VeracryptVolume &volume = VeracryptVolume());

private:
    VeracryptRunnerConfigForm *m_ui;

};

#endif
