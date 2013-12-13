
#include <iostream>
#include <windows.h>
#include <thread>

typedef void (*fun)(int);
typedef void (*funload)(fun*);

bool PlugIn_Load(const char *filename,fun *funct,HINSTANCE *pinst)
{

	HINSTANCE hinstance = LoadLibrary( filename );
	*pinst = hinstance;
	if (!hinstance) {
		char *s;
		DWORD lastErr = GetLastError();
		FormatMessage( FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
                       NULL, lastErr , MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), (char*)&s, 0, NULL );
		printf("*** ERROR: LoadLibrary '%s' err '%s'\n", filename, s);
		LocalFree( s );
		return false;
	}


	void *ptr = (void *)GetProcAddress( hinstance, "load" );
	if (!ptr) {
		char *s;
		FormatMessage( FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
                       NULL, GetLastError() , MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), (char*)&s, 0, NULL );
		printf("*** ERROR: GetProcAddress err '%s'\n", s);
		LocalFree( s );

		FreeLibrary(hinstance);
		return false;
	}

	funload loadFunc = (funload)ptr;
	(*loadFunc)(funct);

	// FIXME: at the moment we never call FreeLibrary() on a loaded plugin

	return true;
}

int main(){
	fun fu=NULL;
	HINSTANCE hinstance;
	if(!PlugIn_Load("liblockedqueuedll.dll",&fu,&hinstance)){
		std::cout << "failed to load" << std::endl;
		return 1;
	}
	std::cout << "fu is:" << fu << std::endl;
	for(int i=1;i<30;i++)
		(*fu)(i);
	std::this_thread::sleep_for(std::chrono::seconds(2));
	if(!FreeLibrary(hinstance))
        std::cout << "FreeLibrary failed" << std::endl;

    std::cout << "End main" << std::endl;
	return 0 ;
}
