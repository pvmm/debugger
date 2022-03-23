#ifndef BREAKPOINTDIALOG_OPENMSX_H
#define BREAKPOINTDIALOG_OPENMSX_H

#include "ui_BreakpointDialog.h"
#include "DebuggerData.h"
#include <QDialog>

struct MemoryLayout;

class DebugSession;
class Symbol;

class BreakpointDialog : public QDialog, private Ui::BreakpointDialog
{
	Q_OBJECT
public:
	BreakpointDialog(const MemoryLayout& ml, DebugSession *session = nullptr, QWidget* parent = nullptr);
	~BreakpointDialog() override;

	Breakpoint::Type type();
	int address();
	int addressEndRange();
	int slot();
	int subslot();
	int segment();
	QString condition();

	void setData(Breakpoint::Type type, int address = -1,
	             qint8 ps = -1, qint8 ss = -1, qint16 segment = -1,
	             int addressEnd = -1, QString condition = QString());

private:
	const MemoryLayout& memLayout;

	DebugSession *debugSession;
	Symbol *currentSymbol;
	int idxSlot, idxSubSlot;
	int value, valueEnd;
	int conditionHeight;
	QCompleter *jumpCompleter, *allCompleter;

private slots:
	void addressChanged(const QString& text);
	void typeChanged(int i);
	void slotChanged(int i);
	void subslotChanged(int i);
	void hasCondition(int state);
};

#endif // BREAKPOINTDIALOG_OPENMSX_H
