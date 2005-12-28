
#ifndef AVRDEVICE_H
#define AVRDEVICE_H


class AVRDevice : public Device {
public:
	AVRDevice(uint32_t sram_start, uint32_t sram_end, uint32_t eeprom_end, uint32_t flash_end, uint32_t io_start, uint32_t io_end);
	uint32_t SRAMStart() const;
	uint32_t SRAMEnd() const;
	uint32_t EEPROMEnd() const;
	uint32_t FLASHEnd() const;
	uint32_t IOStart() const;
	uint32_t IOEnd() const;

private:
	uint32_t sram_start;
	uint32_t sram_end;
	uint32_t eeprom_end;
	uint32_t flash_end;
	uint32_t io_start;
	uint32_t io_end;
};


#endif
