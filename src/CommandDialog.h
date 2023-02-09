#ifndef CONTROLDIALOG_OPENMSX_H
#define CONTROLDIALOG_OPENMSX_H

#include "ui_CommandDialog.h"
#include <QDialog>


struct CommandRef
{
    QString name;
    QString description;
    QString command;
    QString icon;
    int index;

    bool operator<(const CommandRef& ref) const {
        return index < ref.index;
    }
};


class CommandDialog : public QDialog, private Ui::CommandDialog
{
    Q_OBJECT
public:
    CommandDialog(QMap<QString, CommandRef>& commands, QWidget* parent = nullptr);

    QString icon() const;
    QString name() const;
    QString command() const;
    QString function() const;
    void refresh();

private:
    int counter = 0;
    int currentItem = 0;

    int createItem();
    void createCommand();
    void writeCommand(CommandRef& command);

    void onSelectedCommandChanged(int currentRow, int currentCol, int previousRow, int previousCol);
    void onAddButtonClicked();
    void onRemoveButtonClicked();
    void changeTableItem(QTableWidgetItem* item);
    void done();
    void cancel();

    bool userMode = true;
    QMap<QString, CommandRef>& commands;
};

#endif // CONTROLDIALOG_OPENMSX_H
