#include "usbd_hid.h"
#include "usb_device.h"

extern USBD_HandleTypeDef hUsbDeviceFS;

typedef struct
{
	uint8_t MODIFIER;
	uint8_t RESERVED;
	uint8_t KEYCODE1;
	uint8_t KEYCODE2;
	uint8_t KEYCODE3;
	uint8_t KEYCODE4;
	uint8_t KEYCODE5;
	uint8_t KEYCODE6;
}subKeyBoard;

subKeyBoard keyBoardHIDsub = {0,0,0,0,0,0,0,0};


void KeyBoardPrint(char *data, uint16_t length) {
    for (uint16_t count = 0; count < length; count++) {
        // Reset modifier and keycode
        keyBoardHIDsub.MODIFIER = 0x00;
        keyBoardHIDsub.KEYCODE1 = 0x00;
        if (strncmp(&data[count], "UP", 2) == 0) {
			keyBoardHIDsub.KEYCODE1 = 0x52;  // Up arrow
			count += 1;  // Skip next char ('P')
		}
		else if (strncmp(&data[count], "DOWN", 4) == 0) {
			keyBoardHIDsub.KEYCODE1 = 0x51;  // Down arrow
			count += 3;  // Skip next 3 chars ('O', 'W', 'N')
		}
		else if (strncmp(&data[count], "LEFT", 4) == 0) {
			keyBoardHIDsub.KEYCODE1 = 0x50;  // Left arrow
			count += 3;
		}
		else if (strncmp(&data[count], "RIGHT", 5) == 0) {
			keyBoardHIDsub.KEYCODE1 = 0x4F;  // Right arrow
			count += 4;
		}
        // Uppercase letters (A-Z) → SHIFT + key
		else if (data[count] >= 'A' && data[count] <= 'Z') {
            keyBoardHIDsub.MODIFIER = 0x02;  // SHIFT modifier
            keyBoardHIDsub.KEYCODE1 = data[count] - 'A' + 0x04;  // HID 'A' = 0x04
        }
        // Lowercase letters (a-z) → simple key
        else if (data[count] >= 'a' && data[count] <= 'z') {
            keyBoardHIDsub.KEYCODE1 = data[count] - 'a' + 0x04;  // HID 'a' = 0x04 (same as 'A' but without SHIFT)
        }
        // Numbers (0-9)
        else if (data[count] >= '0' && data[count] <= '9') {
            keyBoardHIDsub.KEYCODE1 = (data[count] == '0') ? 0x27 : (data[count] - '1' + 0x1E);  // HID '1' = 0x1E
        }
        // Space
        else if (data[count] == ' ') {
            keyBoardHIDsub.KEYCODE1 = 0x2C;
        }
        // Enter (\n)
        else if (data[count] == '\n') {
            keyBoardHIDsub.KEYCODE1 = 0x28;
        }
        // Symbols requiring SHIFT (e.g., !@#$%^&*())
        else if (strchr("!@#$%^&*()", data[count])) {
            keyBoardHIDsub.MODIFIER = 0x02;  // SHIFT modifier
            switch (data[count]) {
                case '!': keyBoardHIDsub.KEYCODE1 = 0x1E; break;  // SHIFT + 1
                case '@': keyBoardHIDsub.KEYCODE1 = 0x1F; break;  // SHIFT + 2
                case '#': keyBoardHIDsub.KEYCODE1 = 0x20; break;  // SHIFT + 3
                case '$': keyBoardHIDsub.KEYCODE1 = 0x21; break;  // SHIFT + 4
                case '%': keyBoardHIDsub.KEYCODE1 = 0x22; break;  // SHIFT + 5
                case '^': keyBoardHIDsub.KEYCODE1 = 0x23; break;  // SHIFT + 6
                case '&': keyBoardHIDsub.KEYCODE1 = 0x24; break;  // SHIFT + 7
                case '*': keyBoardHIDsub.KEYCODE1 = 0x25; break;  // SHIFT + 8
                case '(': keyBoardHIDsub.KEYCODE1 = 0x26; break;  // SHIFT + 9
                case ')': keyBoardHIDsub.KEYCODE1 = 0x27; break;  // SHIFT + 0
            }
        }
        // Other common symbols (no SHIFT)
        else {
            switch (data[count]) {
                case '-': keyBoardHIDsub.KEYCODE1 = 0x2D; break;
                case '=': keyBoardHIDsub.KEYCODE1 = 0x2E; break;
                case '[': keyBoardHIDsub.KEYCODE1 = 0x2F; break;
                case ']': keyBoardHIDsub.KEYCODE1 = 0x30; break;
                case '\\': keyBoardHIDsub.KEYCODE1 = 0x31; break;
                case ';': keyBoardHIDsub.KEYCODE1 = 0x33; break;
                case '\'': keyBoardHIDsub.KEYCODE1 = 0x34; break;
                case ',': keyBoardHIDsub.KEYCODE1 = 0x36; break;
                case '.': keyBoardHIDsub.KEYCODE1 = 0x37; break;
                case '/': keyBoardHIDsub.KEYCODE1 = 0x38; break;
                // Add more as needed...
            }
        }

        // Send key press
        if (keyBoardHIDsub.KEYCODE1 != 0x00) {
            USBD_HID_SendReport(&hUsbDeviceFS, &keyBoardHIDsub, sizeof(keyBoardHIDsub));
            HAL_Delay(15);  // Key press delay
            // Release key
            keyBoardHIDsub.MODIFIER = 0x00;
            keyBoardHIDsub.KEYCODE1 = 0x00;
            USBD_HID_SendReport(&hUsbDeviceFS, &keyBoardHIDsub, sizeof(keyBoardHIDsub));
            HAL_Delay(25);  // Delay between keys
        }
    }
}

void KeyBoardPrint2(char *data,uint16_t length)
{
	for(uint16_t count=0;count<length;count++)
	{
        // Arrow Keys (case-sensitive)
        if (strncmp(&data[count], "UP", 2) == 0) {
            keyBoardHIDsub.KEYCODE1 = 0x52;  // Up arrow
            count += 1;  // Skip next char ('P')
        }
        else if (strncmp(&data[count], "DOWN", 4) == 0) {
            keyBoardHIDsub.KEYCODE1 = 0x51;  // Down arrow
            count += 3;  // Skip next 3 chars ('O', 'W', 'N')
        }
        else if (strncmp(&data[count], "LEFT", 4) == 0) {
            keyBoardHIDsub.KEYCODE1 = 0x50;  // Left arrow
            count += 3;
        }
        else if (strncmp(&data[count], "RIGHT", 5) == 0) {
            keyBoardHIDsub.KEYCODE1 = 0x4F;  // Right arrow
            count += 4;
        }
        else if(data[count]>=0x41 && data[count]<=0x5A)
		{
			keyBoardHIDsub.MODIFIER=0x02;
			keyBoardHIDsub.KEYCODE1=data[count]-0x3D;
			USBD_HID_SendReport(&hUsbDeviceFS,&keyBoardHIDsub,sizeof(keyBoardHIDsub));
			HAL_Delay(15);
			keyBoardHIDsub.MODIFIER=0x00;
			keyBoardHIDsub.KEYCODE1=0x00;
			USBD_HID_SendReport(&hUsbDeviceFS,&keyBoardHIDsub,sizeof(keyBoardHIDsub));
		}
		else if(data[count]>=0x61 && data[count]<=0x7A)
		{
			keyBoardHIDsub.KEYCODE1=data[count]-0x5D;
			USBD_HID_SendReport(&hUsbDeviceFS,&keyBoardHIDsub,sizeof(keyBoardHIDsub));
			HAL_Delay(15);
			keyBoardHIDsub.KEYCODE1=0x00;
			USBD_HID_SendReport(&hUsbDeviceFS,&keyBoardHIDsub,sizeof(keyBoardHIDsub));
		}
		else if(data[count]==0x20)
		{
			keyBoardHIDsub.KEYCODE1=0x2C;
			USBD_HID_SendReport(&hUsbDeviceFS,&keyBoardHIDsub,sizeof(keyBoardHIDsub));
			HAL_Delay(15);
			keyBoardHIDsub.KEYCODE1=0x00;
			USBD_HID_SendReport(&hUsbDeviceFS,&keyBoardHIDsub,sizeof(keyBoardHIDsub));
		}
		else if(data[count]==0x0A)
		{
			keyBoardHIDsub.KEYCODE1=0x28;
			USBD_HID_SendReport(&hUsbDeviceFS,&keyBoardHIDsub,sizeof(keyBoardHIDsub));
			HAL_Delay(15);
			keyBoardHIDsub.KEYCODE1=0x00;
			USBD_HID_SendReport(&hUsbDeviceFS,&keyBoardHIDsub,sizeof(keyBoardHIDsub));
		}
		HAL_Delay(25);
	}
}
