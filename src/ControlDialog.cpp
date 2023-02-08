#include "ControlDialog.h"
#include <QStandardItemModel>


ControlDialog::ControlDialog(QWidget* parent)
	: QDialog(parent)
{
	setupUi(this);

    tclFunction->setVisible(false);
    connect(cbTclFunction, &QCheckBox::stateChanged, this, &ControlDialog::hasFunction);
}

void ControlDialog::hasFunction(int state)
{
    tclFunction->setVisible(state == Qt::Checked);
}

void ControlDialog::setData(QString icon = {}, QString name = {}, QString command = {}, QString function = {})
{
    Q_UNUSED(icon);
    Q_UNUSED(name);
    Q_UNUSED(command);
    Q_UNUSED(function);
}
