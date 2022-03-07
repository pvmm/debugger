#ifndef BREAKPOINTVIEWER_H
#define BREAKPOINTVIEWER_H

#include <QTabWidget>
#include <QList>
#include "ui_BreakpointViewer.h"

class QPaintEvent;
class Breakpoints;

class BreakpointViewer : public QTabWidget, private Ui::BreakpointViewer
{
	Q_OBJECT;
public:
	BreakpointViewer(QWidget* parent = nullptr);
	void setBreakpoints(Breakpoints* bps);
	void sync();

private:
	Ui::BreakpointViewer* ui;

	// layout
	int frameL, frameR, frameT, frameB;
	int leftRegPos, leftValuePos, rightRegPos, rightValuePos;
	int rowHeight;
	int cursorLoc;
	bool userMode;
	QList<int> bpItemsPos;
	QList<int> wpItemsPos;
	Breakpoints* breakpoints;

	void createBreakpoint(QString address, QString condition = "{}");
	void replaceBreakpoint(int index, const QString address,
		const QString condition = "{}");
	void removeBreakpoint(int index);
	void keepUnusedItems();

public slots:
	void onBpTableItemChanged(QTableWidgetItem *item);
	void on_btnAddBp_clicked();
	void on_btnRemoveBp_clicked();

signals:
	void breakpointsChanged(bool merge);
};

#endif // BREAKPOINTVIEWER_H
