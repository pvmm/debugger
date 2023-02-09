#include "ScopedAssign.h"
#include "CommandDialog.h"
#include <QStandardItemModel>
#include <map>


enum CommandColumns {
    ICON = 0, NAME, DESCRIPTION, COMMAND, HIDDEN, INDEX, MAX
};


CommandDialog::CommandDialog(QMap<QString, CommandRef>& commands, QWidget* parent)
    : QDialog(parent), commands(commands)
{
    setupUi(this);

    twCommands->resizeColumnsToContents();
    twCommands->setColumnHidden(HIDDEN, true);
    twCommands->verticalHeader()->hide();

    connect(btnAdd, &QPushButton::clicked, this, &CommandDialog::onAddButtonClicked);
    connect(btnRemove, &QPushButton::clicked, this, &CommandDialog::onRemoveButtonClicked);
    connect(twCommands, &QTableWidget::itemChanged, this, &CommandDialog::changeTableItem);

    connect(okButton, &QPushButton::clicked, this, &CommandDialog::accept);
    connect(cancelButton, &QPushButton::clicked, this, &CommandDialog::reject);

    refresh();
}

void CommandDialog::refresh()
{
    QList<QPair<CommandRef, QString>> list;

    // set a comparison list
    for (auto key: commands.keys()) {
        list.append(QPair<CommandRef, QString>(commands[key], key));
    }

    // sort by index
    std::sort(std::begin(list), std::end(list));
    auto sa = ScopedAssign(userMode, false);

    // populate
    for (auto& pair: list) {
        createItem();
        writeCommand(pair.first);
    }
}

void CommandDialog::onAddButtonClicked()
{
    auto sa = ScopedAssign(userMode, false);
    createCommand();
}

void CommandDialog::onRemoveButtonClicked()
{
    auto sa = ScopedAssign(userMode, false);
}

int CommandDialog::createItem()
{
    int row = twCommands->rowCount();
    twCommands->setRowCount(row + 1);
    twCommands->selectRow(row);

    auto* item0 = new QTableWidgetItem();
    item0->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled | Qt::ItemIsEditable);
    twCommands->setItem(row, ICON, item0);

    auto* item1 = new QTableWidgetItem();
    item1->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled | Qt::ItemIsEditable);
    twCommands->setItem(row, NAME, item1);

    auto* item2 = new QTableWidgetItem();
    item2->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled | Qt::ItemIsEditable);
    twCommands->setItem(row, DESCRIPTION, item2);

    auto* item3 = new QTableWidgetItem();
    item3->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled | Qt::ItemIsEditable);
    twCommands->setItem(row, COMMAND, item3);

    auto* item4 = new QTableWidgetItem();
    twCommands->setItem(row, HIDDEN, item4);

    return row;
}

void CommandDialog::createCommand()
{
    // get next available name
    QString ctrlName = {};
    for (ctrlName = QString(tr("Command_%1")).arg(counter++);
         commands.find(ctrlName) != commands.end();
         ctrlName = QString(tr("Command_%1")).arg(counter++))
    {}

    int index = createItem();
    CommandRef command{ctrlName, {}, {}, ":/icons/gear.png", index};
    commands.insert(ctrlName, command);
    writeCommand(command);
}

void CommandDialog::writeCommand(CommandRef& command)
{
    auto* item0 = twCommands->item(command.index, ICON);
    item0->setText(command.icon);

    auto* item1 = twCommands->item(command.index, NAME);
    item1->setText(command.name);

    auto* item2 = twCommands->item(command.index, DESCRIPTION);
    item2->setText(command.description);

    auto* item3 = twCommands->item(command.index, COMMAND);
    item3->setText(command.command);

    auto* item4 = twCommands->item(command.index, HIDDEN);
    item4->setText(command.name);

    twCommands->resizeColumnsToContents();
}

void CommandDialog::changeTableItem(QTableWidgetItem* item)
{
    if (!userMode) return;

    int index = twCommands->row(item);
    auto name = twCommands->item(index, NAME)->text();

    // Has the name of the command changed?
    switch (twCommands->column(item)) {
        case ICON:
            commands[name].icon = item->text();
            break;
        case NAME: {
            QString oldName = twCommands->item(index, HIDDEN)->text();
            auto oldCommand = commands.take(oldName);
            oldCommand.name = item->text();
            oldCommand.index = index;
            commands.insert(item->text(), oldCommand);
            break;
        }
	case DESCRIPTION:
            commands[name].description = item->text();
            break;

        case COMMAND:
            commands[name].command = item->text();
            break;
    }
}

void CommandDialog::done()
{
}

void CommandDialog::cancel()
{
}
