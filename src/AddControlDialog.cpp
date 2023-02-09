#include "ControlDialog.h"
#include <QStandardItemModel>

struct ControlRef {
    QString name;
    QString command;
    QString icon;
};

std::map<QString, ControlRef> controls;

ControlDialog::ControlDialog(QWidget* parent)
	: QDialog(parent)
{
	setupUi(this);

    //tclFunction->setVisible(false);
    //connect(cbTclFunction, &QCheckBox::stateChanged, this, &ControlDialog::hasFunction);
    connect(btnAdd, &QPushButton::clicked, this, &ControlDialog::onAddButtonClicked);
    connect(btnRemove, &QPushButton::clicked, this, &ControlDialog::onRemoveButtonClicked);
}

void ControlDialog::hasFunction(int state)
{
    //tclFunction->setVisible(state == Qt::Checked);
}

void ControlDialog::setData(QString icon = {}, QString name = {}, QString command = {}, QString function = {})
{
    Q_UNUSED(icon);
    Q_UNUSED(name);
    Q_UNUSED(command);
    Q_UNUSED(function);
}

void ControlDialog::onAddButtonClicked()
{
    createControl();
}

void ControlDialog::onRemoveButtonClicked()
{
}

void ControlDialog::createControl()
{
    QString controlName = QString(tr("Control_%1")).arg(counter++);

    auto item = new QListWidgetItem(controlName, lwControlList);
    item->setSelected(true);

    ControlRef control{controlName, {}, {}};
    controls.try_emplace(controlName, control);

    setCurrentControl(item);
}

void ControlDialog::setCurrentControl(QListWidgetItem* item)
{
    auto& control = controls[item->text()];

    //btnIcon->setImage(control.icon);
    ctrlName->setText(control.name);
    tclCommand->setText(control.command);
}

