#include "ScopedAssign.h"
#include "ControlDialog.h"
#include <QStandardItemModel>
#include <map>


enum ControlColumns {
    ICON = 0, NAME, DESCRIPTION, COMMAND, HIDDEN, INDEX, MAX
};


ControlDialog::ControlDialog(QMap<QString, ControlRef>& controls, QWidget* parent)
    : QDialog(parent), controls(controls)
{
    setupUi(this);

    twControls->resizeColumnsToContents();
    twControls->setColumnHidden(HIDDEN, true);
    twControls->verticalHeader()->hide();

    connect(btnAdd, &QPushButton::clicked, this, &ControlDialog::onAddButtonClicked);
    connect(btnRemove, &QPushButton::clicked, this, &ControlDialog::onRemoveButtonClicked);
    connect(twControls, &QTableWidget::itemChanged, this, &ControlDialog::changeTableItem);

    connect(okButton, &QPushButton::clicked, this, &ControlDialog::accept);
    connect(cancelButton, &QPushButton::clicked, this, &ControlDialog::reject);

    refresh();
}

void ControlDialog::refresh()
{
    QList<QPair<ControlRef, QString>> list;

    // set a comparison list
    for (auto key: controls.keys()) {
        list.append(QPair<ControlRef, QString>(controls[key], key));
    }

    // sort by index
    std::sort(std::begin(list), std::end(list));
    auto sa = ScopedAssign(userMode, false);

    // populate
    for (auto& pair: list) {
        createItem();
        writeControl(pair.first);
    }
}

void ControlDialog::onAddButtonClicked()
{
    auto sa = ScopedAssign(userMode, false);
    createControl();
}

void ControlDialog::onRemoveButtonClicked()
{
    auto sa = ScopedAssign(userMode, false);
}

int ControlDialog::createItem()
{
    int row = twControls->rowCount();
    twControls->setRowCount(row + 1);
    twControls->selectRow(row);

    auto* item0 = new QTableWidgetItem();
    item0->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled | Qt::ItemIsEditable);
    twControls->setItem(row, ICON, item0);

    auto* item1 = new QTableWidgetItem();
    item1->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled | Qt::ItemIsEditable);
    twControls->setItem(row, NAME, item1);

    auto* item2 = new QTableWidgetItem();
    item2->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled | Qt::ItemIsEditable);
    twControls->setItem(row, DESCRIPTION, item2);

    auto* item3 = new QTableWidgetItem();
    item3->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled | Qt::ItemIsEditable);
    twControls->setItem(row, COMMAND, item3);

    auto* item4 = new QTableWidgetItem();
    twControls->setItem(row, HIDDEN, item4);

    return row;
}

void ControlDialog::createControl()
{
    // get next available name
    QString ctrlName = {};
    for (ctrlName = QString(tr("Control_%1")).arg(counter++);
         controls.find(ctrlName) != controls.end();
         ctrlName = QString(tr("Control_%1")).arg(counter++))
    {}

    int index = createItem();
    ControlRef control{ctrlName, {}, {}, ":/icons/gear.png", index};
    controls.insert(ctrlName, control);
    writeControl(control);
}

void ControlDialog::writeControl(ControlRef& control)
{
    auto* item0 = twControls->item(control.index, ICON);
    item0->setText(control.icon);

    auto* item1 = twControls->item(control.index, NAME);
    item1->setText(control.name);

    auto* item2 = twControls->item(control.index, DESCRIPTION);
    item2->setText(control.description);

    auto* item3 = twControls->item(control.index, COMMAND);
    item3->setText(control.command);

    auto* item4 = twControls->item(control.index, HIDDEN);
    item4->setText(control.name);

    twControls->resizeColumnsToContents();
}

void ControlDialog::changeTableItem(QTableWidgetItem* item)
{
    if (!userMode) return;

    int index = twControls->row(item);
    auto name = twControls->item(index, NAME)->text();

    // Has the name of the control changed?
    switch (twControls->column(item)) {
        case ICON:
            controls[name].icon = item->text();
            break;
        case NAME: {
            QString oldName = twControls->item(index, HIDDEN)->text();
            auto oldControl = controls.take(oldName);
            oldControl.name = item->text();
            oldControl.index = index;
            controls.insert(item->text(), oldControl);
            break;
        }
	case DESCRIPTION:
            controls[name].description = item->text();
            break;

        case COMMAND:
            controls[name].command = item->text();
            break;
    }
}

void ControlDialog::done()
{
}

void ControlDialog::cancel()
{
}
