#include <windows.h>
#include "../main/guifuncs.h"
#include "../main/win/GUI_LogWindow.h"

static DWORD RenderThreadId;
static HANDLE RenderThreadHanlde;

static char * get_renderjobspath()
{
    static char path[1024];
    strcpy(path, get_currentpath());
    strcat(path, "RenderJobs/");
    return path;
}

static char * get_renderoutpath()
{
    static char path[1024];
    strcpy(path, get_currentpath());
    strcat(path, "RenderOut/");
    return path;
}

static void checkJobs() {
    ShowInfo("PlaybackRendering: Checking Queue");
}

static DWORD WINAPI thread_render(LPVOID lpParam) {
    static HANDLE notifyHandle;
    char * tPath = get_renderjobspath();
    
    notifyHandle = FindFirstChangeNotification(tPath, FALSE, FILE_NOTIFY_CHANGE_FILE_NAME);
    
    checkJobs();
    
    while(TRUE) {
        WaitForSingleObject(notifyHandle, INFINITE);   
        checkJobs();                        
        FindNextChangeNotification(notifyHandle);
    }
    ExitThread(0);
}

void render_controller_init(void) {
    ShowInfo("Emu thread: Creating Playback Render thread...");
    RenderThreadHanlde = CreateThread(NULL, 0, thread_render, NULL, 0, &RenderThreadId);
}

void render_controller_destroy() {
    TerminateThread(RenderThreadHanlde, 0);
}
