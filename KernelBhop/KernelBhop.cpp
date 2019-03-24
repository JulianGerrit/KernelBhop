/* Cheat that uses a driver for reading / writing virtual memory,
instead of using Win32API Functions. Written By Zer0Mem0ry,
https://www.youtube.com/watch?v=sJdBtPosWQs */

#include <iostream>

#include "KeInterface.h"




int main()
{
	KeInterface Driver("\\\\.\\kernelhop");

	// Get address of client.dll & pid of csgo from our driver
	DWORD ProcessId = Driver.GetTargetPid();
	DWORD ClientAddress = Driver.GetClientModule();

	// Get address of localplayer
	DWORD LocalPlayer = Driver.ReadVirtualMemory<DWORD>(ProcessId, ClientAddress + LOCAL_PLAYER);

	// address of inground
	DWORD InGround = Driver.ReadVirtualMemory<DWORD>(ProcessId,
		LocalPlayer + FFLAGS);

	// check that addresses were found


	std::cout << "Found csgo Process Id: " << ProcessId << std::endl;
	std::cout << "Found client.dll ClientBase: 0x" << std::uppercase
		<< std::hex << ClientAddress << std::endl;
	std::cout << "Found LocalPlayer in client.dll: 0x" << std::uppercase
		<< std::hex << LocalPlayer << std::endl;
	std::cout << "Found PlayerInGround: 0x" << std::uppercase
		<< std::hex << InGround << std::endl;

	while (true)
	{
		// Constant checking so that the cheat doesnt have to be restarted every new game
		DWORD LocalPlayer = Driver.ReadVirtualMemory<DWORD>(ProcessId, ClientAddress + LOCAL_PLAYER);
		DWORD InGround = Driver.ReadVirtualMemory<DWORD>(ProcessId, LocalPlayer + FFLAGS);
		// Check if space is down & player is in ground
		if ((GetAsyncKeyState(VK_SPACE) & 0x8000) && (InGround & 1 == 1))
		{
			// Jump
			Driver.WriteVirtualMemory<int>(ProcessId, ClientAddress + FORCE_JUMP, 0x5);
			Sleep(30);
			// Restore
			Driver.WriteVirtualMemory<int>(ProcessId, ClientAddress + FORCE_JUMP, 0x4);
			
		}
		Sleep(1);
	}
    return 0;
}

