#include "VDPDataStore.h"

class VDPDataStoreVersionCheck : public SimpleCommand
{
public:
	VDPDataStoreVersionCheck(VDPDataStore& dataStore_ ) : SimpleCommand("debug desc {physical VRAM}"),
		dataStore(dataStore_)
	{
	}

	virtual void replyOk(const QString& message)
	{
		dataStore.old_version = false;
		dataStore.got_version = true;
		dataStore.refresh();
		delete this;
	}
	virtual void replyNok(const QString& message)
	{
		dataStore.old_version = true;
		dataStore.got_version = true;
		dataStore.refresh();
		delete this;
	}
private:
	VDPDataStore& dataStore;
};

VDPDataStore::VDPDataStore()
{
	vram = new unsigned char[0x20000 + 32 + 16 + 64];
	palette = vram + 0x20000;
	statusRegs = palette + 32;
	regs = statusRegs + 16;
	memset(vram,0x00,0x20000+32+16+64);
	got_version=false;
	old_version=false;
	CommClient::instance().sendCommand(new VDPDataStoreVersionCheck(*this));
}

VDPDataStore::~VDPDataStore()
{
	delete[] vram;
}

void VDPDataStore::refresh()
{
	if (!got_version) return;

	QString req;
	if (old_version){
	  req = QString(
		"debug_bin2hex "
		"[ debug read_block {VRAM} 0 0x20000 ]"
		"[ debug read_block {VDP palette} 0 32 ]"
		"[ debug read_block {VDP status regs} 0 16 ]"
		"[ debug read_block {VDP regs} 0 64 ]"
		);
	} else {
	  req = QString(
		"debug_bin2hex "
		"[ debug read_block {physical VRAM} 0 0x20000 ]"
		"[ debug read_block {VDP palette} 0 32 ]"
		"[ debug read_block {VDP status regs} 0 16 ]"
		"[ debug read_block {VDP regs} 0 64 ]"
		);
	};
	new SimpleHexRequest(req
		,  unsigned(0x20000+32+16+64), vram , *this);
}

void VDPDataStore::DataHexRequestReceived()
{
	emit dataRefreshed();
}

unsigned char* VDPDataStore::getVramPointer()
{
	return vram;
}
unsigned char* VDPDataStore::getPalettePointer()
{
	return palette;
}
unsigned char* VDPDataStore::getRegsPointer()
{
	return regs;
}
unsigned char* VDPDataStore::getStatusRegsPointer()
{
	return statusRegs;
}

VDPDataStore& VDPDataStore::instance()
{
	static VDPDataStore oneInstance;
	return oneInstance;
}

