#ifndef CONTROLDIALOG_OPENMSX_H
#define CONTROLDIALOG_OPENMSX_H

#include "ui_ControlDialog.h"
#include <QDialog>


struct ControlRef
{
    QString name;
    QString description;
    QString command;
    QString icon;
    int index;

    bool operator<(const ControlRef& ref) const {
        return index < ref.index;
    }
};


class ControlDialog : public QDialog, private Ui::ControlDialog
{
    Q_OBJECT
public:
    ControlDialog(QMap<QString, ControlRef>& controls, QWidget* parent = nullptr);

    QString icon() const;
    QString name() const;
    QString command() const;
    QString function() const;
    void refresh();

private:
    int counter = 0;
    int currentItem = 0;

    int createItem();
    void createControl();
    void writeControl(ControlRef& control);

    void onSelectedControlChanged(int currentRow, int currentCol, int previousRow, int previousCol);
    void onAddButtonClicked();
    void onRemoveButtonClicked();
    void changeTableItem(QTableWidgetItem* item);
    void done();
    void cancel();

    bool userMode = true;
    QMap<QString, ControlRef>& controls;
};

#endif // CONTROLDIALOG_OPENMSX_H
