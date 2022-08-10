#pragma comment(lib, "opengl32.lib")
#pragma comment(lib, "shell32.lib")

#define WIN32_LEAN_AND_MEAN

#include <windows.h>
#include <shellapi.h>

#define WINDOW_CLASS_NAME "doki-window"
#define WINDOW_STYLE (WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX)

#ifdef NDEBUG
// Stuff needed when compiling without linking the c runtime
EXTERN_C int _fltused = 0; // Needed in order to use floating point types
#endif

static HWND hwnd;
static HDC hdc;
static HGLRC hrc;
static HMODULE hgldll;
static HANDLE hfile;
static HANDLE hevent;
static OVERLAPPED overlapped;

static LONG_PTR WINAPI window_proc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam) {
	static char filename[MAX_PATH];
	switch (msg) {
	case WM_DROPFILES:
		DragQueryFileA((HDROP)wparam, 0, filename, sizeof(filename));
		CloseHandle(hfile);
		hfile = CreateFileA(filename, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL | FILE_FLAG_OVERLAPPED, NULL);
		overlapped.Internal = 0;
		overlapped.InternalHigh = 0;
		overlapped.Offset = -1;
		overlapped.OffsetHigh = 0;
		overlapped.hEvent = hevent;
		return 0;
	case WM_CLOSE:
		CloseHandle(hevent);
		CloseHandle(hfile);
		wglDeleteContext(hrc);
		ReleaseDC(hwnd, hdc);
		DestroyWindow(hwnd);
		return 0;
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	}
	return DefWindowProc(hwnd, msg, wparam, lparam);
}

int main() {

	hgldll = GetModuleHandleA("opengl32.dll");
	hfile = INVALID_HANDLE_VALUE;
	hevent = CreateEventA(NULL, FALSE, FALSE, NULL);
	
	SetProcessDpiAwarenessContext(DPI_AWARENESS_CONTEXT_PER_MONITOR_AWARE_V2);

	WNDCLASS wc = { 0 };
	wc.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC /*| CS_DBLCLKS*/;
	wc.lpfnWndProc = window_proc;
	wc.hInstance = GetModuleHandle(NULL);
	wc.hIcon = LoadIcon(NULL, IDI_WINLOGO);
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.lpszClassName = WINDOW_CLASS_NAME;
	RegisterClass(&wc);

	hwnd = CreateWindowA(
		WINDOW_CLASS_NAME, "", WINDOW_STYLE,
		0, 0, 0, 0,
		NULL, NULL, GetModuleHandle(NULL), NULL);

	hdc = GetDC(hwnd);
	PIXELFORMATDESCRIPTOR pfd = { 0 };
	pfd.nSize = sizeof(pfd);
	pfd.nVersion = 1;
	pfd.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
	pfd.iPixelType = PFD_TYPE_RGBA;
	pfd.cColorBits = 32;
	pfd.iLayerType = PFD_MAIN_PLANE;
	int pf = ChoosePixelFormat(hdc, &pfd);
	SetPixelFormat(hdc, pf, &pfd);

	DragAcceptFiles(hwnd, true);

	void application_start();
	application_start();

	ShowWindow(hwnd, SW_SHOW);

	MSG msg = { 0 };
	while (true) {

		while (PeekMessage(&msg, 0, 0, 0, PM_REMOVE)) {
			TranslateMessage(&msg);
			DispatchMessage(&msg);
			if (msg.message == WM_QUIT) break;
		}

		if (msg.message == WM_QUIT) break;

		void application_update();
		application_update();
		SwapBuffers(hdc);
	}

	ExitProcess(0);
}

void os_set_window_title(const char* title) {
	SetWindowText(hwnd, title);
}

void os_set_window_size(long width, long height) {

	RECT window_rect = { 0, 0, width, height };
	AdjustWindowRect(&window_rect, WINDOW_STYLE, false);
	long w = window_rect.right - window_rect.left;
	long h = window_rect.bottom - window_rect.top;

	HWND desktop_hwnd = GetDesktopWindow();
	RECT desktop_rect;
	GetClientRect(desktop_hwnd, &desktop_rect);
	long x = max(0, desktop_rect.right / 2 - w / 2);
	long y = max(0, desktop_rect.bottom / 2 - h / 2);

	SetWindowPos(hwnd, NULL, x, y, w, h, SWP_NOACTIVATE | SWP_NOZORDER);
}

bool os_file_dropped() {
	return hfile != INVALID_HANDLE_VALUE;
}

bool os_file_read(void* buffer, long count) {
	overlapped.Offset++;
	return ReadFile(hfile, buffer, (DWORD)count, NULL, &overlapped);
}

bool os_file_is_ready() {
	DWORD dummy;
	return GetOverlappedResult(hfile, &overlapped, &dummy, FALSE);
}

void os_init_gl(long major_version, long minor_version) {

	HGLRC temp_hrc = wglCreateContext(hdc);
	wglMakeCurrent(hdc, temp_hrc);

	typedef HGLRC(WINAPI* PFNWGLCREATECONTEXTATTRIBSARBPROC)(HDC hDC, HGLRC hShareContext, const int* attribList);
	PFNWGLCREATECONTEXTATTRIBSARBPROC wglCreateContextAttribsARB = (PFNWGLCREATECONTEXTATTRIBSARBPROC)wglGetProcAddress("wglCreateContextAttribsARB");

	int attributes[] = {
		0x2091, major_version, // WGL_CONTEXT_MAJOR_VERSION_ARB
		0x2092, minor_version, // WGL_CONTEXT_MINOR_VERSION_ARB
		0
	};

	hrc = wglCreateContextAttribsARB(hdc, NULL, attributes);
	wglMakeCurrent(NULL, NULL);
	wglDeleteContext(temp_hrc);
	wglMakeCurrent(hdc, hrc);

	typedef BOOL(WINAPI* PFNWGLSWAPINTERVALEXTPROC)(int interval);
	PFNWGLSWAPINTERVALEXTPROC wglSwapIntervalEXT = (PFNWGLSWAPINTERVALEXTPROC)wglGetProcAddress("wglSwapIntervalEXT");
	wglSwapIntervalEXT(1);
}

void* os_get_gl_proc(const char* name) {
	FARPROC proc = GetProcAddress(hgldll, name);
	if (proc) return (void*)proc;
	return (void*)wglGetProcAddress(name);
}
