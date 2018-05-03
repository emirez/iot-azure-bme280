#include <EEPROM.h>

// Read parameters from EEPROM or Serial
void readCredentials()
{
    int ssidAddr = 0;
    int passAddr = ssidAddr + SSID_LEN;
    int connectionStringAddr = passAddr + SSID_LEN;

    // malloc for parameters
    ssid = (char *)malloc(SSID_LEN);
    pass = (char *)malloc(PASS_LEN);
    connectionString = (char *)malloc(CONNECTION_STRING_LEN);

    // try to read out the credential information, if failed, the length should be 0.
    // int ssidLength = EEPROMread(ssidAddr, ssid);
    // int passLength = EEPROMread(passAddr, pass);
    // int connectionStringLength = EEPROMread(connectionStringAddr, connectionString);
    int ssidLength = 0;
    int passLength = 0;
    int connectionStringLength = 0;
    
    if (ssidLength > 0 && passLength > 0 && connectionStringLength > 0 && !needEraseEEPROM())
    {
        return;
    }

    ssid = "belkin.36ef";
    EEPROMWrite(ssidAddr, ssid, strlen(ssid));
    pass = "34e966fb";
    EEPROMWrite(passAddr, pass, strlen(pass));
    connectionString = "HostName=TFHub.azure-devices.net;DeviceId=TFNode;SharedAccessKey=aOfwZ2dkf1wL05e/bZqagSZINRF48FhNg29WNdlkkxM=";
    EEPROMWrite(connectionStringAddr, connectionString, strlen(connectionString));
}

bool needEraseEEPROM()
{
        clearParam();
        return true;
}

// reset the EEPROM
void clearParam()
{
    char data[EEPROM_SIZE];
    memset(data, '\0', EEPROM_SIZE);
    EEPROMWrite(0, data, EEPROM_SIZE);
}

#define EEPROM_END 0
#define EEPROM_START 1
void EEPROMWrite(int addr, char *data, int size)
{
    EEPROM.begin(EEPROM_SIZE);
    // write the start marker
    EEPROM.write(addr, EEPROM_START);
    addr++;
    for (int i = 0; i < size; i++)
    {
        EEPROM.write(addr, data[i]);
        addr++;
    }
    EEPROM.write(addr, EEPROM_END);
    EEPROM.commit();
    EEPROM.end();
}

// read bytes from addr util '\0'
// return the length of read out.
int EEPROMread(int addr, char *buf)
{
    EEPROM.begin(EEPROM_SIZE);
    int count = -1;
    char c = EEPROM.read(addr);
    addr++;
    if (c != EEPROM_START)
    {
        return 0;
    }
    while (c != EEPROM_END && count < EEPROM_SIZE)
    {
        c = (char)EEPROM.read(addr);
        count++;
        addr++;
        buf[count] = c;
    }
    EEPROM.end();
    return count;
}
