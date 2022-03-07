#include <QPainter>
#include <QPaintEvent>
#include <QMessageBox>
#include <QToolTip>
#include <QDebug>
#include <cassert>
#include "BreakpointViewer.h"
#include "ui_BreakpointViewer.h"
#include "DebuggerData.h"
#include "Convert.h"
#include "CommClient.h"
#include "OpenMSXConnection.h"

enum BpTableColumns {
	BP_ENABLED = 0,
	BP_ADDRESS,
	BP_CONDITION,
	BP_PRIMARY,
	BP_SECONDARY,
	BP_SEGMENT,
	BP_ONCE,
};

enum WpTableColumns {
	WP_ENABLED = 0,
	WP_TYPE,
	WP_REGION,
	WP_CONDITION,
	WP_PRIMARY,
	WP_SECONDARY,
	WP_SEGMENT,
};

BreakpointViewer::BreakpointViewer(QWidget* parent)
	: QTabWidget(parent),
	  ui(new Ui::BreakpointViewer)
{
	setupUi(this);
	bpTableWidget->sortByColumn(BP_ADDRESS, Qt::AscendingOrder);
	bpTableWidget->setSelectionBehavior(QAbstractItemView::SelectRows);

	userMode = true;

	connect(bpTableWidget, &QTableWidget::itemChanged, this,
			&BreakpointViewer::onBpTableItemChanged);
}

void BreakpointViewer::createBreakpoint(QString address, QString condition)
{
	if (!address.length())
		return;

	qDebug() << "createBreakpoint() called. [" << address << "]";
	address = address.simplified();
	condition = condition.simplified();
	const QString cmdStr = Breakpoints::createSetCommand(Breakpoints::BREAKPOINT,
		stringToValue(address), -1, -1, -1, -1, condition);
	auto command = new Command(cmdStr,
		[this, address, condition] (const QString result) {
			emit breakpointsChanged(false);
		}
	);
	CommClient::instance().sendCommand(command);
}

void BreakpointViewer::replaceBreakpoint(int index,
										 const QString address,
										 const QString condition)
{
	qDebug() << "replaceBreakpoint() called.";
	QString id = breakpoints->getAttribute(index, "id").toString();
	const QString cmdStr = breakpoints->createRemoveCommand(id);
	qDebug() << "cmdStr =" << cmdStr;
	auto command = new Command(cmdStr,
			[this, index, address, condition] (const QString& result) {
					qDebug() << "result =" << result;
					createBreakpoint(address, condition);
			}
	);
	CommClient::instance().sendCommand(command);
}

void BreakpointViewer::removeBreakpoint(int index)
{
	QString id = breakpoints->getAttribute(index, "id").toString();
	qDebug() << "removeBreakpoint(" << id << ") called.";
	QString cmdStr = Breakpoints::createRemoveCommand(id);
	qDebug() << "cmdStr =" << cmdStr;

	auto command = new Command(cmdStr,
		[this] (const QString result) {
			qDebug() << "result =" << result;
			emit breakpointsChanged(false);
	});
	CommClient::instance().sendCommand(command);
}

void BreakpointViewer::onBpTableItemChanged(QTableWidgetItem* item)
{
	qDebug() << "onBpTableItemChanged() called";
	if (!userMode) return;

	// if try modify a bp instead of creating a new one
	bool createBp = false;

	// check if bp is enabled
	int row = bpTableWidget->row(item);
	QTableWidgetItem* enabledItem = bpTableWidget->item(row, BP_ENABLED);
	bool enabled = enabledItem->checkState() == Qt::Checked;

	// update breakpoints
	switch (bpTableWidget->column(item)) {
	case BP_ENABLED:
		createBp = enabled;
		break;
	case BP_ADDRESS: {
		int index = bpItemsPos.indexOf(row);
		int value;

		if ((value = stringToValue(item->text())) == -1) {
			userMode = false;
			if (index != -1)
				item->setText(breakpoints->getAttribute(index, "address").toString());
			else
				item->setText("");
			userMode = true;
			return;
		}
	void drawValue(QPainter& p, int id, int x, int y);

		qDebug() << "BLA2!";
		// update field without triggering onBpTableItemChanged again
		userMode = true;
		item->setText(hexValue(value, 4));
		userMode = true;
		if (!enabled) return;
		break;
	}
	case BP_PRIMARY:
		userMode = false;
		item->setText(item->text().simplified());
		userMode = true;
		if (!enabled) return;
		break;
	case BP_SECONDARY:
		userMode = false;
		item->setText(item->text().simplified());
		userMode = true;
		if (!enabled) return;
		break;
	case BP_SEGMENT:
		userMode = false;
		item->setText(item->text().simplified());
		userMode = true;
		if (!enabled) return;
		break;
	case BP_CONDITION:
		userMode = false;
		item->setText(item->text().simplified());
		userMode = true;
		if (!enabled) return;
		break;
	}

	qDebug() << "update breakpoints:";
	QString cmdStr;
	int index;

	if (enabled) {
		QString addressStr = bpTableWidget->item(row, BP_ADDRESS) ?
			bpTableWidget->item(row, BP_ADDRESS)->text() : "";
		QString conditionStr = bpTableWidget->item(row, BP_CONDITION) ?
			bpTableWidget->item(row, BP_CONDITION)->text() : "";

		if (createBp) {
			if (!addressStr.length()) {
				enabledItem->setCheckState(Qt::Unchecked);
			} else {
				createBreakpoint(addressStr, conditionStr);
			}
		} else if ((index = bpItemsPos.indexOf(row)) != -1) {
			qDebug() << "replacing breakpoint:" << index;
			replaceBreakpoint(index, addressStr, conditionStr);
		}
	} else if ((index = bpItemsPos.indexOf(row)) != -1) {
			qDebug() << "removing breakpoint:" << index;
			removeBreakpoint(index);
	}
}

void BreakpointViewer::setBreakpoints(Breakpoints* bps)
{
	qDebug() << "BreakpointViewer::setBreakpoints() called";
	breakpoints = bps;
}

void BreakpointViewer::keepUnusedItems()
{
	int row = 0;

	while (row < bpTableWidget->rowCount()) {
		QTableWidgetItem* enabledItem = bpTableWidget->item(row, BP_ENABLED);
		bool enabled = enabledItem->checkState() == Qt::Checked;
		if (enabled)
			bpTableWidget->removeRow(row);
		else
			row++;
	}

	while (row < wpTableWidget->rowCount()) {
		QTableWidgetItem* enabledItem = wpTableWidget->item(row, WP_ENABLED);
		bool enabled = enabledItem->checkState() == Qt::Checked;
		if (enabled)
			wpTableWidget->removeRow(row);
		else
			row++;
	}
}

void BreakpointViewer::sync()
{
	bpItemsPos.clear();
	wpItemsPos.clear();

	userMode = false;
	keepUnusedItems();

	QString id;
	int bpRow = bpTableWidget->rowCount();
	int wpRow = wpTableWidget->rowCount();
	int index = breakpoints->findBreakpoint(0);
	if (index == -1) index = breakpoints->findNextBreakpoint();

	if (index != -1) {
		QTableWidgetItem* item;

		do {
			if (breakpoints->isBreakpoint(breakpoints->getAddress(index), &id)) {
				bpTableWidget->setRowCount(bpRow + 1);

				item = new QTableWidgetItem();
				item->setFlags(Qt::ItemIsUserCheckable | Qt::ItemIsSelectable
						| Qt::ItemIsEnabled);
				item->setCheckState(Qt::Checked);
				bpTableWidget->setItem(bpRow, BP_ENABLED, item);

				QString address = hexValue(
					breakpoints->getAttribute(index, "address").toInt(), 4);
				item = new QTableWidgetItem(address);
				item->setTextAlignment(Qt::AlignCenter);
				bpTableWidget->setItem(bpRow, BP_ADDRESS, item);
				qDebug() << bpTableWidget->item(bpRow, 1)->text();

				QString condition = breakpoints->getAttribute(index, "condition").toString();
				item = new QTableWidgetItem(condition);
				item->setTextAlignment(Qt::AlignCenter);
				bpTableWidget->setItem(bpRow, BP_CONDITION, item);

				bpTableWidget->setSortingEnabled(true);
				int row = bpTableWidget->row(item);

				bpItemsPos.append(row);
				bpTableWidget->setSortingEnabled(false);

				bpRow = bpTableWidget->rowCount();
			}
			else if (breakpoints->isWatchpoint(breakpoints->getAddress(index), &id)) {
				wpTableWidget->setRowCount(wpRow + 1);

				item = new QTableWidgetItem();
				item->setFlags(Qt::ItemIsUserCheckable | Qt::ItemIsSelectable
						| Qt::ItemIsEnabled);
				item->setCheckState(Qt::Checked);
				bpTableWidget->setItem(wpRow, WP_ENABLED, item);

				QString type = breakpoints->getAttribute(index, "type").toString();
				item = new QTableWidgetItem(type);
				item->setTextAlignment(Qt::AlignCenter);
				bpTableWidget->setItem(wpRow, WP_TYPE, item);

				QString address = hexValue(
					breakpoints->getAttribute(index, "address").toInt(), 4);
				QString regionEnd = hexValue(
					breakpoints->getAttribute(index, "regionEnd").toInt(), 4);
				QString region = QString("{%1 %2}").arg(address).arg(regionEnd);
				item = new QTableWidgetItem(region);
				item->setTextAlignment(Qt::AlignCenter);
				bpTableWidget->setItem(wpRow, WP_REGION, item);

				QString condition = breakpoints->getAttribute(index, "condition").toString();
				item = new QTableWidgetItem(condition);
				item->setTextAlignment(Qt::AlignCenter);
				bpTableWidget->setItem(wpRow, WP_CONDITION, item);

				wpTableWidget->setSortingEnabled(true);
				int row = bpTableWidget->row(item);

				wpItemsPos.append(row);
				wpTableWidget->setSortingEnabled(false);

				wpRow = wpTableWidget->rowCount();
			} else {
				assert(false);
			}
		}
		while ((index = breakpoints->findNextBreakpoint()) != -1);
	}

	userMode = true;
}

void BreakpointViewer::on_btnAddBp_clicked()
{
	int row = bpTableWidget->rowCount();
	bpTableWidget->setRowCount(row + 1);
	bpTableWidget->selectRow(row);

	userMode = false;
	QTableWidgetItem* item = new QTableWidgetItem();
	item->setFlags(Qt::ItemIsUserCheckable | Qt::ItemIsSelectable
			| Qt::ItemIsEnabled);
	item->setCheckState(Qt::Unchecked);
	bpTableWidget->setItem(row, BP_ENABLED, item);
	userMode = true;
}

void BreakpointViewer::on_btnRemoveBp_clicked()
{
	bpTableWidget->removeRow(bpTableWidget->currentRow());
}
