#include <EEPROM.h>



String readEEPROM()
{
	
	char str_buf[20] = "";
	uint addr = 0;

	EEPROM.begin(EEPROM_SIZE);

	EEPROM.get(addr, str_buf);
	
	return str_buf;
}
bool writeEEPROM(String buf)
{
	char str_buf[20] = "";
	uint addr = 0;

	buf.toCharArray(str_buf, 20);

	EEPROM.begin(EEPROM_SIZE);

	EEPROM.put(addr, str_buf);

	return EEPROM.commit();
}

void eraseEEPROM()
{

}

