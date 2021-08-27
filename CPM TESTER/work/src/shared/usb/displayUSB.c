#include <windows.h>
#include <stdio.h>
#include "Usbioctl.h"

SECURITY_ATTRIBUTES SA;

const char ClassName[] [20] = 
{
	"Reserved", 
	"Audio", 
	"Communications", 
	"Human Interface",
	"Monitor", 
	"Physical Interface", 
	"Power", 
	"Printer",
	"Storage", 
	"Hub", 
	"Vendor Specific", 
	"*ILLEGAL VALUE*"
};

typedef struct SHUB_DESCRIPTOR 
{
	UCHAR bDescriptorLength; 
	UCHAR bDescriptorType; 
	UCHAR bNumberOfPorts;
	UCHAR wHubCharacteristics[2]; 
	UCHAR bPowerOnToPowerGood; 
	UCHAR bHubControlCurrent;
	UCHAR bRemoveAndPowerMask[64];
}HUB_DESCRIPTOR;

typedef struct SNODE_INFORMATION 
{
	USB_HUB_NODE NodeType; 
	HUB_DESCRIPTOR HubDescriptor; 
	BOOLEAN HubIsBusPowered;
}NODE_INFORMATION;

typedef struct SDEVICE_DESCRIPTOR 
{
	UCHAR bLength; 
	UCHAR bDescriptorType; 
	UCHAR bcdUSB[2];
	UCHAR bDeviceClass; 
	UCHAR bDeviceSubClass; 
	UCHAR bDeviceProtocol;
	UCHAR bMaxPacketSize0; 
	UCHAR idVendor[2]; 
	UCHAR idProduct[2];
	UCHAR bcdDevice[2]; 
	UCHAR iManufacturer; 
	UCHAR iProduct;
	UCHAR iSerialNumber; 
	UCHAR bNumConfigurations;
}DEVICE_DESCRIPTOR;

typedef struct SNODE_CONNECTION_INFORMATION 
{
	ULONG ConnectionIndex; 
	DEVICE_DESCRIPTOR DeviceDescriptor; 
	UCHAR CurrentConfigurationValue;
	BOOLEAN LowSpeed; 
	BOOLEAN DeviceIsHub; 
	UCHAR DeviceAddress[2];
	UCHAR NumberOfOpenPipes[4]; 
	UCHAR ConnectionStatus[4]; 
	USB_PIPE_INFO PipeList[32];
}NODE_CONNECTION_INFORMATION;

typedef struct SDESCRIPTOR_REQUEST
{
	ULONG ConnectionIndex;
	struct 
	{	
		UCHAR bmRequest; 
		UCHAR bRequest; 
		UCHAR wValue[2]; 
		UCHAR wIndex[2]; 
		UCHAR wLength[2];
	} SetupPacket;
	UCHAR Data[2048];
}DESCRIPTOR_REQUEST;

USHORT DisplayStringDescriptor (HANDLE HubHandle, ULONG PortIndex, USHORT LanguageID, UCHAR Index) 
{
	DESCRIPTOR_REQUEST Packet;
	DWORD BytesReturned;
	BOOL Success;

	if (LanguageID == 0) 
	{ // Get the language ID
		memset(&Packet, 0, sizeof(Packet));
		Packet.ConnectionIndex = PortIndex;
		Packet.SetupPacket.bmRequest = 0x80;
		Packet.SetupPacket.bRequest = USB_REQUEST_GET_DESCRIPTOR;
		Packet.SetupPacket.wValue[1] = USB_STRING_DESCRIPTOR_TYPE;
		Packet.SetupPacket.wLength[0] = 4;
		Success = DeviceIoControl(HubHandle, IOCTL_USB_GET_DESCRIPTOR_FROM_NODE_CONNECTION, &Packet,
							      sizeof(Packet), &Packet, sizeof(Packet), &BytesReturned, NULL);
		if (!Success) 
			printf(" *** ERROR *** String Descriptor 0 not returned, ErrorCode = %d\n",	GetLastError());
		
		LanguageID = Packet.Data[2] + (Packet.Data[3] << 8);
	}
	memset(&Packet, 0, sizeof(Packet));
	Packet.ConnectionIndex = PortIndex;
	Packet.SetupPacket.bmRequest = 0x80;
	Packet.SetupPacket.bRequest = USB_REQUEST_GET_DESCRIPTOR;
	Packet.SetupPacket.wValue[1] = USB_STRING_DESCRIPTOR_TYPE;
	Packet.SetupPacket.wValue[0] = Index;
	Packet.SetupPacket.wIndex[0] = LanguageID & 0xFF;
	Packet.SetupPacket.wIndex[1] = (LanguageID >> 8) & 0xFF;
	Packet.SetupPacket.wLength[0] = 255;
	
	Success = DeviceIoControl(HubHandle, IOCTL_USB_GET_DESCRIPTOR_FROM_NODE_CONNECTION, &Packet,
							  sizeof(Packet), &Packet, sizeof(Packet), &BytesReturned, NULL);
	if (!Success) 
		printf(" *** ERROR *** String Descriptor %d not returned. ErrorCode = %d\n", Index,	GetLastError());
	
	printf(" = %ws", &Packet.Data[2]);
	
	return LanguageID;
}

USHORT DisplayDeviceDescriptor (HANDLE HubHandle, ULONG PortIndex, USHORT LanguageID, PUCHAR BufferPtr) 
{
	UCHAR LowByte;

	printf("Device Descriptor");
	BufferPtr--; // Backup pointer to prepare for pre-increment
	printf("\n bLength %2.2x", *++BufferPtr);
	printf("\n bDescriptorType %2.2x", *++BufferPtr);
	LowByte = *++BufferPtr;
	printf("\n bcdUSB %4.4x", LowByte + (*++BufferPtr << 8));
	printf("\n bDeviceClass %2.2x", *++BufferPtr);
	printf("\n bDeviceSubClass %2.2x", *++BufferPtr);
	printf("\n bDeviceProtocol %2.2x", *++BufferPtr);
	printf("\n bMaxEP0Size %2.2x", *++BufferPtr);
	LowByte = *++BufferPtr;
	printf("\n wVendorID %4.4x", LowByte + (*++BufferPtr << 8));
	LowByte = *++BufferPtr;
	printf("\n wProductID %4.4x", LowByte + (*++BufferPtr << 8));
	LowByte = *++BufferPtr;
	printf("\n wDeviceID %4.4x", LowByte + (*++BufferPtr << 8));
	printf("\n iManufacturer %2.2x", *++BufferPtr);
	if (*BufferPtr != 0) 
		LanguageID = DisplayStringDescriptor(HubHandle, PortIndex, LanguageID, *BufferPtr);
	printf("\n iProduct %2.2x", *++BufferPtr);
	if (*BufferPtr != 0) 
		LanguageID = DisplayStringDescriptor(HubHandle, PortIndex, LanguageID, *BufferPtr);
	printf("\n iSerialNumber %2.2x", *++BufferPtr);
	if (*BufferPtr != 0) 
		LanguageID = DisplayStringDescriptor(HubHandle, PortIndex, LanguageID, *BufferPtr);
	printf("\n bNumConfigurations %2.2x\n", *++BufferPtr);
	return LanguageID;
}

USHORT DisplayConfigurationDescriptor(HANDLE HubHandle, ULONG PortIndex, USHORT LanguageID) 
{
	DWORD BytesReturned;
	BOOL Success;
	UCHAR LowByte;
	DESCRIPTOR_REQUEST Packet;
	int i;
	PUCHAR BufferPtr;
	UCHAR Length;

	printf("\nConfiguration Descriptor");
	// First need to get the configuration descriptor
	memset(&Packet, 0, sizeof(Packet));
	Packet.ConnectionIndex = PortIndex;
	Packet.SetupPacket.bmRequest = 0x80;
	Packet.SetupPacket.bRequest = USB_REQUEST_GET_DESCRIPTOR;
	Packet.SetupPacket.wValue[1] = USB_CONFIGURATION_DESCRIPTOR_TYPE;
	Packet.SetupPacket.wLength[1] = 1; // Using a 2K buffer
	
	Success = DeviceIoControl(HubHandle, IOCTL_USB_GET_DESCRIPTOR_FROM_NODE_CONNECTION, &Packet,
							  sizeof(Packet), &Packet, sizeof(Packet), &BytesReturned, NULL);
	if (!Success) 
		printf(" *** ERROR *** Configuration Descriptor not returned. ErrorCode = %d\n", GetLastError());

	BufferPtr = &Packet.Data[0];
	Length = *BufferPtr;
	
	while (Length != 0) 
	{
		UCHAR Type = *++BufferPtr;
		switch (Type) 
		{
			case 2:
				printf("\n bLength %2.2x", Length);
				printf("\n bDescriptorType %2.2x = Configuration Header", Type);
				LowByte = *++BufferPtr;
				printf("\n wTotalLength %4.4x", LowByte + (*++BufferPtr << 8));
				printf("\n bNumInterfaces %2.2x", *++BufferPtr);
				printf("\n bConfigValue %2.2x", *++BufferPtr);
				printf("\n iConfiguration %2.2x", *++BufferPtr);
				if (*BufferPtr != 0) LanguageID = DisplayStringDescriptor(HubHandle, PortIndex, LanguageID,
				*BufferPtr);
				printf("\n bmAttributes %2.2x", *++BufferPtr);
				LowByte = *++BufferPtr;
				printf("\n bMaxPower %2.2x = %d mA", LowByte, (LowByte << 1));
				break;
			case 4:
				printf("\n bLength %2.2x", Length);
				printf("\n bDescriptorType %2.2x = Interface Descriptor", Type);
				printf("\n bInterfaceNum %2.2x", *++BufferPtr);
				printf("\n bAlternateSetting %2.2x", *++BufferPtr);
				printf("\n bNumEndpoints %2.2x", *++BufferPtr);
				LowByte = *++BufferPtr;
				if ((LowByte > 9) & (LowByte < 255)) LowByte = 11;
				if (LowByte == 255) LowByte = 10;
				printf("\n bInterfaceClass %2.2x = %s", *BufferPtr, ClassName[LowByte]);
				printf("\n bSubClass %2.2x", *++BufferPtr);
				printf("\n bProtocol %2.2x", *++BufferPtr);
				printf("\n iInterface %2.2x", *++BufferPtr);
				if (*BufferPtr != 0) LanguageID = DisplayStringDescriptor(HubHandle, PortIndex, LanguageID,
				*BufferPtr);
				break;
			case 5:
				printf("\n bLength %2.2x", Length);
				printf("\n bDescriptorType %2.2x = Endpoint Descriptor", Type);
				printf("\n bEndpointAddress %2.2x", *++BufferPtr);
				printf("\n bmAttributes %2.2x", *++BufferPtr);
				LowByte = *++BufferPtr;
				printf("\n wMaxPacketSize %4.4x", LowByte + (*++BufferPtr << 8));
				printf("\n bInterval %2.2x", *++BufferPtr);
				break;
			case 0x21:
				printf("\n bLength %2.2x", Length);
				printf("\n bDescriptorType %2.2x = HID Descriptor", Type);
				LowByte = *++BufferPtr;
				printf("\n wHIDversion %4.4x", LowByte + (*++BufferPtr << 8));
				printf("\n bCountryCode %2.2x", *++BufferPtr);
				printf("\n bHIDDescriptorCount %2.2x", *++BufferPtr);
				printf("\n bHIDReportType %2.2x", *++BufferPtr);
				LowByte = *++BufferPtr;
				printf("\n wHIDReportLength %4.4x", LowByte + (*++BufferPtr << 8));
				break;
			default:
				printf("\nUnknown descriptor with Length = %2.2xH and Type = %2.2xH", Length, Type);
				BufferPtr-=2; // Back up to start of descriptor
				for (i = 0; i < Length; i++) 
				{
					if ((i % 16) == 0)
						printf("\n");
					printf("%2.2x ", *++BufferPtr);
				}
				break;
		}
		Length = *++BufferPtr;
		printf("\n");
	}
	return LanguageID;
}

void GetPortData(HANDLE HubHandle, UCHAR PortCount, int HubDepth) 
{
	DWORD BytesReturned;
	BOOL Success;
	int i;
	ULONG PortIndex;
	USHORT LanguageID;
	UCHAR ThisDevice, PortStatus;
	char ConnectedHubName[256] = "\\\\.\\";
	HANDLE ConnectedHubHandle;
	NODE_INFORMATION NodeInformation;
	NODE_CONNECTION_INFORMATION ConnectionInformation;

	struct {ULONG ConnectionIndex; ULONG ActualLength; WCHAR Name[256];} ConnectedHub;
	
	// Iterate over the ports to discover what is connected to each one
	for (PortIndex = 1; PortIndex < (ULONG)PortCount + 1; PortIndex++) 
	{
		LanguageID = 0; // Reset for each port
		ConnectionInformation.ConnectionIndex = PortIndex;
		
		Success = DeviceIoControl(HubHandle, IOCTL_USB_GET_NODE_CONNECTION_INFORMATION, &ConnectionInformation,
					sizeof(ConnectionInformation), &ConnectionInformation, sizeof(ConnectionInformation), &BytesReturned, NULL);

		if (!Success) 
			printf(" *** ERROR *** Node connection information not returned\n");
		
		PortStatus = ConnectionInformation.ConnectionStatus[0]; // Save some typing!
		
		ThisDevice = (PortStatus == DeviceConnected) ? ConnectionInformation.DeviceAddress[0] : 0;
		
		// Create an indented display so that hubs and their connections are more easily seen
		// First the common header
		// printf("%2.2x", ThisDevice);
		for (i=0; i<HubDepth; i++) 
			printf("+1");
		
		printf(" Port[%d] = ", PortIndex);
		
		// Now the connection specific information
		if (PortStatus != DeviceConnected) 
		{
			printf("%s\n", ConnectionInformation.ConnectionStatus[PortStatus]);
		}
		else 
		{ // have a device or a hub connected to this port
			if (!ConnectionInformation.DeviceIsHub) 
			{
				// There is an I/O device connected. Print out it's descriptors
				// Note that many current devices do not respond correctly if ConfigID != 0. So only request the first configuration
				printf("I/O device connected\n");
				LanguageID = DisplayDeviceDescriptor(HubHandle, PortIndex, LanguageID, &ConnectionInformation.DeviceDescriptor.bLength);

				LanguageID = DisplayConfigurationDescriptor(HubHandle, PortIndex, LanguageID);
			}
			else 
			{
				// There is a hub connected and we need to iterate over it's ports
				printf("Hub connected\n");

				// Get the system name of the connected hub so that we can make a connection to it
				ConnectedHub.ConnectionIndex = PortIndex;

				Success = DeviceIoControl(HubHandle, IOCTL_USB_GET_NODE_CONNECTION_NAME, &ConnectedHub,
										  sizeof(ConnectedHub), &ConnectedHub, sizeof(ConnectedHub), &BytesReturned, NULL);
				
				if (!Success) 
					printf(" *** ERROR *** Node connection name not returned\n");
				WideCharToMultiByte(CP_ACP, 0, &ConnectedHub.Name[0], (ConnectedHub.ActualLength)/2,
									&ConnectedHubName[4], 252, NULL, NULL);
				
				ConnectedHubHandle = CreateFile(ConnectedHubName, GENERIC_WRITE, FILE_SHARE_WRITE, &SA,
												OPEN_EXISTING, 0, NULL);
				
				// Connected hub is open. Collect the node information
				Success = DeviceIoControl(ConnectedHubHandle, IOCTL_USB_GET_NODE_INFORMATION, &NodeInformation,
										  sizeof(NodeInformation), &NodeInformation, sizeof(NodeInformation), &BytesReturned, NULL);
				if (!Success) 
					printf(" *** ERROR *** Node information not returned\n");
				
				GetPortData(ConnectedHubHandle, NodeInformation.HubDescriptor.bNumberOfPorts, HubDepth+1);
				
				CloseHandle(ConnectedHubHandle);
			};
		};
	};
}

DWORD EnumerateHostController(HANDLE HostControllerHandle) 
{
	DWORD BytesReturned;
	BOOL Success;
	struct {ULONG Length; WCHAR Name[256];} UnicodeName;
	char RootHubName[256] = "\\\\.\\";
	HANDLE RootHubHandle;
	NODE_INFORMATION NodeInformation;

	// First get the system name of the host controller for display
	Success = DeviceIoControl(HostControllerHandle, IOCTL_GET_HCD_DRIVERKEY_NAME, &UnicodeName,	sizeof(UnicodeName), &UnicodeName, sizeof(UnicodeName), &BytesReturned, NULL);
	
	if (!Success) 
		return GetLastError();

	printf("System name is %ws\n", &UnicodeName.Name[0]);

	// Now get the system name of it's root hub for interrogation
	Success = DeviceIoControl(HostControllerHandle, IOCTL_USB_GET_ROOT_HUB_NAME, &UnicodeName, sizeof(UnicodeName), &UnicodeName, sizeof(UnicodeName), &BytesReturned, NULL);
	
	// Now open the root hub. Need to construct a char name from "\\.\" + UnicodeName
	WideCharToMultiByte(CP_ACP, 0, &UnicodeName.Name[0], (UnicodeName.Length)/2, &RootHubName[4], 252, NULL, NULL);
	
	RootHubHandle = CreateFile(RootHubName, GENERIC_WRITE, FILE_SHARE_WRITE, &SA, OPEN_EXISTING, 0, NULL);
	
	if (RootHubHandle == INVALID_HANDLE_VALUE) 
		return GetLastError();
	
	// Root hub is open. Collect the node information
	Success = DeviceIoControl(RootHubHandle, IOCTL_USB_GET_NODE_INFORMATION, &NodeInformation,
	
	sizeof(NodeInformation), &NodeInformation, sizeof(NodeInformation), &BytesReturned, NULL);
	
	if (!Success) 
		return GetLastError();
	
	// Can now iterate over the ports
	GetPortData(RootHubHandle, NodeInformation.HubDescriptor.bNumberOfPorts, 0);
	
	CloseHandle(RootHubHandle);

	return 0;
}

int main(int argc, char* argv[]) 
{
	HANDLE HostControllerHandle;
	char HostControllerName[] = "\\\\.\\HCD0";
	int i;
	DWORD ErrorCode = 0;
	FILE *stream;

	SA.nLength = sizeof(SECURITY_ATTRIBUTES);
	SA.lpSecurityDescriptor = NULL;
	SA.bInheritHandle = FALSE;

	stream = freopen("C:\\DisplayUSB.txt", "w", stdout);
	printf("USB Design By Example: Display currently attached USB Devices\n");

	for (i=0; i<10; i++) 
	{
		HostControllerName[7] = i + '0';
		HostControllerHandle = CreateFile(HostControllerName, GENERIC_WRITE, FILE_SHARE_WRITE, &SA, OPEN_EXISTING, 0, NULL);
		
		if ((HostControllerHandle != INVALID_HANDLE_VALUE) & (ErrorCode == 0)) 
		{
			printf("\nHost Controller %s found. ", HostControllerName);
			ErrorCode = EnumerateHostController(HostControllerHandle);
		
			CloseHandle(HostControllerHandle);
		}
	}
	fclose(stream);
	
	system("notepad C:\\DisplayUSB.txt");

	return ErrorCode;
}