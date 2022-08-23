#pragma comment(lib, "opengl32.lib")
#pragma comment(lib, "shell32.lib")

#define WIN32_LEAN_AND_MEAN

#include <windows.h>
#include <shellapi.h>

#ifndef NDEBUG
#include <cstdio>
#endif

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
static HMENU hmenu;
static HKEY hkey;
static HANDLE hfile;
static HANDLE hevent;
static OVERLAPPED overlapped;
static long menu_result;
static double time_delta;

static LONG_PTR WINAPI window_proc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam) {
	static char filename[MAX_PATH];
	switch (msg) {
	case WM_RBUTTONDOWN:
		POINT p;
		p.x = LOWORD(lparam);
		p.y = HIWORD(lparam);
		ClientToScreen(hwnd, &p);
		menu_result = TrackPopupMenu(hmenu, TPM_TOPALIGN | TPM_LEFTALIGN | TPM_NONOTIFY | TPM_RETURNCMD, p.x, p.y, 0, hwnd, NULL);
		menu_result--;
		return 0;
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
		RegCloseKey(hkey);
		DestroyMenu(hmenu);
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
	hmenu = CreatePopupMenu();
	RegCreateKeyExA(HKEY_CURRENT_USER, "Software\\DokiCom", 0, NULL, REG_OPTION_NON_VOLATILE, KEY_WRITE | KEY_QUERY_VALUE, NULL, &hkey, NULL);
	hfile = INVALID_HANDLE_VALUE;
	hevent = CreateEventA(NULL, FALSE, FALSE, NULL);
	menu_result = -1;
	
	SetProcessDpiAwarenessContext(DPI_AWARENESS_CONTEXT_PER_MONITOR_AWARE_V2);

	HMODULE hinstance = GetModuleHandle(NULL);

	WNDCLASS wc = { 0 };
	wc.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC /*| CS_DBLCLKS*/;
	wc.lpfnWndProc = window_proc;
	wc.hInstance = hinstance;
	wc.hIcon = LoadIcon(hinstance, MAKEINTRESOURCE(101));
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

	LARGE_INTEGER frequency;
	LARGE_INTEGER t_last;
	LARGE_INTEGER t_now;
	QueryPerformanceFrequency(&frequency);
	QueryPerformanceCounter(&t_last);

	MSG msg = { 0 };
	while (true) {

		while (PeekMessage(&msg, 0, 0, 0, PM_REMOVE)) {
			TranslateMessage(&msg);
			DispatchMessage(&msg);
			if (msg.message == WM_QUIT) break;
		}

		if (msg.message == WM_QUIT) break;

		QueryPerformanceCounter(&t_now);
		time_delta = (t_now.QuadPart - t_last.QuadPart) / (double)frequency.QuadPart;
		t_last = t_now;
#ifndef NDEBUG
		printf("%f\n", time_delta);
#endif

		void application_update();
		application_update();
		menu_result = -1;
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

void os_add_menu_item(long id, const char* string) {
	AppendMenuA(hmenu, MF_STRING, (UINT_PTR)id + 1, string);
}

void os_add_menu_separator() {
	AppendMenuA(hmenu, MF_SEPARATOR, 0, 0);
}

long os_get_menu_result() {
	return menu_result;
}

long os_read_config(const char* key, long default_value) {
	long result = default_value;
	DWORD size = sizeof(result);
	RegGetValueA(hkey, NULL, key, RRF_RT_REG_DWORD, NULL, &result, &size);
	return result;
}

void os_write_config(const char* key, long value) {
	RegSetValueExA(hkey, key, 0, REG_DWORD, (BYTE*)&value, sizeof(value));
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
