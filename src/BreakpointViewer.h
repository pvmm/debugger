#ifndef BREAKPOINTVIEWER_H
#define BREAKPOINTVIEWER_H

#include "ui_BreakpointViewer.h"
#include "DebuggerData.h"
#include <QList>
#include <QTabWidget>
#include <optional>
#include <tuple>

class QPaintEvent;
class Breakpoints;
class DebugSession;

class BreakpointViewer : public QTabWidget, private Ui::BreakpointViewer
{
	Q_OBJECT
public:
	enum Type { BREAKPOINT, WATCHPOINT, CONDITION, ALL };

	BreakpointViewer(DebugSession& session, QWidget* parent = nullptr);
	void setBreakpoints(Breakpoints* bps);

	void onSymbolTableChanged();
	void on_btnAddBp_clicked();
	void on_btnRemoveBp_clicked();
	void on_btnAddWp_clicked();
	void on_btnRemoveWp_clicked();
	void on_btnAddCn_clicked();
	void on_btnRemoveCn_clicked();

	void setRunState();
	void setBreakState();
	void refresh();

signals:
	void contentsUpdated();

private:
	void setTextField(BreakpointViewer::Type type, int row, int column, const QString& value, const QString& tooltip = {});

	std::optional<AddressRange> parseSymbolOrValue(const QString& field) const;

	QString findSymbolOrValue(uint16_t address) const;

	std::optional<AddressRange> parseLocationField(std::optional<int> bpIndex,
	                                               BreakpointViewer::Type type,
	                                               const QString& field,
	                                               const QString& combo = {});
	Slot parseSlotField(std::optional<int> bpIndex, const QString& field);
	std::optional<uint8_t> parseSegmentField(std::optional<int> bpIndex, const QString& field);
	void changeTableItem(BreakpointViewer::Type type, QTableWidgetItem* item);
	void createComboBox(int row);
	Breakpoint::Type readComboBox(int row);
	int  createTableRow(BreakpointViewer::Type type, int row = -1);
	void fillTableRowLocation(BreakpointViewer::Type type, int row, const QString& location);
	void fillTableRow(BreakpointViewer::Type type, int row, int bpIndex);
	std::optional<Breakpoint> parseTableRow(BreakpointViewer::Type type, int row);

	std::optional<int> getTableIndexByRow(BreakpointViewer::Type type, int row) const;
	void createBreakpoint(BreakpointViewer::Type type, int row);
	void _handleSyncError(const QString& error);
	void _handleKeyAlreadyExists();
	void _handleKeyNotFound();
	void _createBreakpoint(BreakpointViewer::Type type, int row);
	void _createCondition(int row);

	void replaceBreakpoint(BreakpointViewer::Type type, int row);
	void removeBreakpoint(BreakpointViewer::Type type, int row, bool removeLocal = false);
	void setBreakpointChecked(BreakpointViewer::Type type, int row, Qt::CheckState state);
	void onAddBtnClicked(BreakpointViewer::Type type);
	void onRemoveBtnClicked(BreakpointViewer::Type type);
	void stretchTable(BreakpointViewer::Type type = BreakpointViewer::ALL);

	std::optional<int> findBreakpointIndex(BreakpointViewer::Type type, int row) const;
	std::optional<int> findBreakpointRow(BreakpointViewer::Type type, const QString& id) const;

	void changeCurrentWpType(int row, int index);
	void disableSorting(BreakpointViewer::Type type = BreakpointViewer::ALL);
	void changeBpTableItem(QTableWidgetItem* item);
	void changeWpTableItem(QTableWidgetItem* item);
	void changeCnTableItem(QTableWidgetItem* item);
	void on_itemPressed(QTableWidgetItem* item);
	void on_headerClicked(int index);
	void refreshTableRow(int bpIndex, BreakpointViewer::Type type, int row);

private:
	Ui::BreakpointViewer* ui;
	DebugSession& debugSession;

	QTableWidget* tables[BreakpointViewer::ALL];

	bool disableRefresh = false;
	bool userMode = true;
	bool runState;
	bool conditionsMsg = false;
	Breakpoints* breakpoints;
};

#endif // BREAKPOINTVIEWER_H
