#ifndef CONFIGURATIONDIALOG_H
#define CONFIGURATIONDIALOG_H

#include <QDialog>

#include "Application.h"
#include "Core/Configuration.h"

#include "ui_ConfigurationDialog.h"

class ConfigurationDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ConfigurationDialog( const Configuration&, QWidget* parent );
    Configuration configuration() const;

private Q_SLOTS:
    void on_databaseLocationButton_clicked();
    void on_databaseLocation_textChanged( const QString& text );
    void on_nameLineEdit_textChanged( const QString& text );

    virtual void accept();

private:
    void checkInput();
    Configuration m_config;
    Ui::ConfigurationDialog m_ui;
};
#endif
