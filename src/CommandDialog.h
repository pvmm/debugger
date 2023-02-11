#ifndef CONTROLDIALOG_OPENMSX_H
#define CONTROLDIALOG_OPENMSX_H

#include "ui_CommandDialog.h"
#include <QDialog>


struct CommandRef
{
    QString name;
    QString description;
    QString source;
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
    CommandDialog(QList<CommandRef>& commands, QWidget* parent = nullptr);

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
    void writeCommand(const CommandRef& command);

    // void onSelectedCommandChanged(int currentRow, int currentCol, int previousRow, int previousCol);
    void onAddButtonClicked();
    void onRemoveButtonClicked();
    void changeTableItem(const QTableWidgetItem* item);
    void accept() override;
    void reject() override;

    bool userMode = true;
    QList<CommandRef>& commands;
};

#endif // CONTROLDIALOG_OPENMSX_H
