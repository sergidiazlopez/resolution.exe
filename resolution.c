#include <windows.h>
#include <stdio.h>
#include <tchar.h>



// Function to change the desktop resolution
void SetResolution(int width, int height, int frequency) {
    DEVMODE dm;
    dm.dmSize = sizeof(DEVMODE);
    dm.dmPelsWidth = width;
    dm.dmPelsHeight = height;
    dm.dmDisplayFrequency = frequency;
    dm.dmFields = DM_PELSWIDTH | DM_PELSHEIGHT | DM_DISPLAYFREQUENCY;

    if (ChangeDisplaySettings(&dm, CDS_FULLSCREEN) != DISP_CHANGE_SUCCESSFUL) {
        printf("Failed to change resolution\n");
        return;
    }
}

void _tmain(int argc, TCHAR* argv[])
{
    if (argc != 5)
    {
        printf("Not enough arguments!");
        return;
    }

    // Hide the console
    FreeConsole();

    // Store the previous settings
    DEVMODE originalMode;
    EnumDisplaySettings(NULL, ENUM_CURRENT_SETTINGS, &originalMode);

    // Change the resolution
    SetResolution(_ttoi(argv[1]), _ttoi(argv[2]), _ttoi(argv[3]));

    // Execute the application
    STARTUPINFO si;
    PROCESS_INFORMATION pi;
    ZeroMemory(&si, sizeof(si));
    si.cb = sizeof(si);
    ZeroMemory(&pi, sizeof(pi));

    // Start the child process. 
    // From https://learn.microsoft.com/ca-es/windows/win32/procthread/creating-processes?redirectedfrom=MSDN
    if (!CreateProcess(NULL,   // No module name (use command line)
        argv[4],        // Command line
        NULL,           // Process handle not inheritable
        NULL,           // Thread handle not inheritable
        FALSE,          // Set handle inheritance to FALSE
        0,              // No creation flags
        NULL,           // Use parent's environment block
        NULL,           // Use parent's starting directory 
        &si,            // Pointer to STARTUPINFO structure
        &pi)           // Pointer to PROCESS_INFORMATION structure
        )
    {
        printf("Failed to start the application\n");
        ChangeDisplaySettings(&originalMode, 0);
        return;
    }

    // Wait for the application to exit
    WaitForSingleObject(pi.hProcess, INFINITE);

    // Restore the original resolution
    ChangeDisplaySettings(&originalMode, 0);

    // Close handles
    CloseHandle(pi.hProcess);
    CloseHandle(pi.hThread);

    return;
}
