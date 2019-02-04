#include <EEPROM.h>

//EEPROM NO LONGER USED

String readEEPROM()
{
	
	char str_buf[20] = "";
	uint addr = 0;

	EEPROM.begin(512);

	EEPROM.get(addr, str_buf);
	
	return str_buf;
}
bool writeEEPROM(String buf)
{
	char str_buf[20] = "";
	uint addr = 0;

	buf.toCharArray(str_buf, 20);

	EEPROM.begin(512);

	EEPROM.put(addr, str_buf);

	return EEPROM.commit();
}

