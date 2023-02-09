#ifndef CONTROLDIALOG_OPENMSX_H
#define CONTROLDIALOG_OPENMSX_H

#include "ui_ControlDialog.h"
#include <QDialog>

class ControlDialog : public QDialog, private Ui::ControlDialog
{
	Q_OBJECT
public:
	ControlDialog(QWidget* parent = nullptr);

    QString icon() const;
    QString name() const;
    QString command() const;
    QString function() const;

	void setData(QString icon, QString name, QString command, QString function);

private:
    int counter = 0;

    void createControl();
    void setCurrentControl(QListWidgetItem* item);

    void onAddButtonClicked();
    void onRemoveButtonClicked();

    void iconChanged(const QString& file);
    void nameChanged(const QString& name);
    void commandChanged(const QString& name);
    void functionChanged(const QString& name);

	void hasFunction(int state);
};

#endif // CONTROLDIALOG_OPENMSX_H
