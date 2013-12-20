
#include <iostream>
#include "lockedqueue.hpp"
#include <windows.h>
struct DiskIOThread
{

	locked_queue<int> lq;
	std::thread mThread;

	~DiskIOThread()
	{
		lq.cancel(true);
		std::cout << mThread.joinable() << " is joinable" << std::endl;
		mThread.join();
		std::cout << mThread.joinable() << " is joinable" << std::endl;
	}

	void launchThread()
	{
		using namespace std;
		lq.cancel(false);
		mThread = thread(&DiskIOThread::ioThreadFunc, this) ;
		//mThread = move( thread( bind(&DiskIOThread::ioThreadFunc, this) ) );
	}

	void Write(int data)
	{
		lq.push(data);
	}

	void ioThreadFunc()
	{
		while (!lq.cancelled) {
			int val = lq.pop();
			std::cout << val << std::endl;
		}
		std::cout << "exit ioThreadFunc" << std::endl;
	}
};

DiskIOThread gDiskI;
void Write(int data){
	(gDiskI).Write(data);
}

typedef void (*fun)(int);
extern "C" __declspec(dllexport) void load(fun *function){
	//new(&gDiskI) DiskIOThread();
	//gDiskI = new DiskIOThread();
	(gDiskI).launchThread();
	(*function) = &Write;
}
extern "C" __declspec(dllexport) void unload(){
	/*(*gDiskI).~DiskIOThread();*/

}


extern "C" __declspec(dllexport) BOOL APIENTRY DllMain(
    HINSTANCE hinstDLL,  // handle to DLL module
    DWORD fdwReason,     // reason for calling function
    LPVOID lpReserved )  // reserved
{
    int a;
    // Perform actions based on the reason for calling.
    switch( fdwReason )
    {
        case DLL_PROCESS_ATTACH:
         // Initialize once for each new process.
         // Return FALSE to fail DLL load.
         std::cout << "DLL_PROCESS_ATTACH" << std::endl;
        // (gDiskI).launchThread();
         a = 1;
            break;

        case DLL_THREAD_ATTACH:
         // Do thread-specific initialization.
         std::cout << "DLL_THREAD_ATTACH" << std::endl;
         a = 1;
            break;

        case DLL_THREAD_DETACH:
         // Do thread-specific cleanup.
         std::cout << "DLL_THREAD_DETACH" << std::endl;
         a = 1;
            break;

        case DLL_PROCESS_DETACH:
         // Perform any necessary cleanup.
         std::cout << "DLL_PROCESS_DETACH" << std::endl;
         a = 1;
            break;
    }
    return TRUE;  // Successful DLL_PROCESS_ATTACH.
}

