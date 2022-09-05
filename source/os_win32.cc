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
static long menu_result;
static BYTE file_buffer[256 * 1024];
static bool file_is_read;
static bool shift_held;
static bool control_held;
static bool alt_held;
static bool key_matrix[256];

static bool key_down_vk(int vk) {
	return (GetAsyncKeyState(vk) & 0x8000) != 0;
}

static bool key_down_ch(char ch) {
	SHORT r = VkKeyScanA(ch);
	int vk = r & 0x00FF;
	bool shift = (r & 0x0100) != 0;
	bool control = (r & 0x0200) != 0;
	bool alt = (r & 0x0400) != 0;
	if (shift && !shift_held) return false;
	if (control && !control_held) return false;
	if (alt && !alt_held) return false;
	return key_down_vk(vk);
}

static void read_file(LPCWSTR filename) {
	HANDLE hfile = CreateFileW(filename, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	DWORD file_size = GetFileSize(hfile, NULL);
	DWORD bytes_read;
	if (ReadFile(hfile, file_buffer, min(file_size, sizeof(file_buffer)), &bytes_read, NULL))
		file_is_read = true;
	CloseHandle(hfile);
}

static LONG_PTR WINAPI window_proc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam) {
	static wchar_t filename[MAX_PATH];
	
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
		DragQueryFileW((HDROP)wparam, 0, filename, sizeof(filename) - 1);
		read_file(filename);
		SetFocus(hwnd);
		return 0;
	case WM_CLOSE:
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
	menu_result = -1;
	file_is_read = false;

	int argc = 0;
	LPWSTR* argv = CommandLineToArgvW(GetCommandLineW(), &argc);
	if (argc > 1)
		read_file(argv[1]);
	
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
	LARGE_INTEGER t_add;
	QueryPerformanceFrequency(&frequency);
	QueryPerformanceCounter(&t_last);

	MSG msg = { 0 };
	while (true) {
		QueryPerformanceCounter(&t_now);
		double dt = (t_now.QuadPart - t_last.QuadPart) / (double)frequency.QuadPart;
		if (dt < 0.014) {
			while (true) {
				QueryPerformanceCounter(&t_add);
				double dt = (t_add.QuadPart - t_last.QuadPart) / (double)frequency.QuadPart;
				if (dt >= 0.016)
					break;
			}
		}

#ifndef NDEBUG
		printf("%f\n", dt);
#endif
		t_last = t_now;

		while (PeekMessage(&msg, 0, 0, 0, PM_REMOVE)) {
			TranslateMessage(&msg);
			DispatchMessage(&msg);
			if (msg.message == WM_QUIT) break;
		}

		if (msg.message == WM_QUIT) break;
		
		shift_held = key_down_vk(VK_SHIFT);
		control_held = key_down_vk(VK_CONTROL);
		alt_held = key_down_vk(VK_RMENU);
		for (int i = 0x00; i <= 0x09; i++)
			key_matrix[i] = key_down_ch('0' + i);
		for (int i = 0x0A; i <= 0x23; i++)
			key_matrix[i] = key_down_ch('A' + i - 0x0A) || key_down_ch('a' + i - 0x0A);
		static char symbols[] = "_ .,:;!?'\"#@+-*/%&|=<>()[]{}";
		for (int i = 0x24; i <= 0x3F; i++)
			key_matrix[i] = key_down_ch(symbols[i - 0x24]);
		static int special[] = { VK_RETURN, VK_TAB, VK_BACK, VK_DELETE, VK_LEFT, VK_RIGHT, VK_UP, VK_DOWN };
		for (int i = 0x40; i <= 0x47; i++)
			key_matrix[i] = key_down_vk(special[i - 0x40]);

		void application_update();
		application_update();
		menu_result = -1;
		SwapBuffers(hdc);
	}

	ExitProcess(0);
}

void os_window_set_title(const char* title) {
	SetWindowText(hwnd, title);
}

void os_window_set_size(long width, long height) {

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

void os_menu_add_item(long id, const char* string) {
	AppendMenuA(hmenu, MF_STRING, (UINT_PTR)id + 1, string);
}

void os_menu_add_separator() {
	AppendMenuA(hmenu, MF_SEPARATOR, 0, 0);
}

void os_menu_set_enabled(long id, bool enabled) {
	EnableMenuItem(hmenu, id + 1, enabled ? MF_ENABLED : MF_DISABLED);
}

void os_menu_set_checked(long id, bool checked) {
	CheckMenuItem(hmenu, id + 1, checked ? MF_CHECKED : MF_UNCHECKED);
}

long os_menu_result() {
	return menu_result;
}

long os_config_read(const char* key, long default_value) {
	long result = default_value;
	DWORD size = sizeof(result);
	RegGetValueA(hkey, NULL, key, RRF_RT_REG_DWORD, NULL, &result, &size);
	return result;
}

void os_config_write(const char* key, long value) {
	RegSetValueExA(hkey, key, 0, REG_DWORD, (BYTE*)&value, sizeof(value));
}

bool os_file_is_read() {
	return file_is_read;
}

unsigned char* os_file_data() {
	return file_buffer;
}

bool os_input_key_is_down(unsigned char key) {
	return key_matrix[key];
}

void os_gl_init(long major_version, long minor_version) {

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

void* os_gl_proc(const char* name) {
	FARPROC proc = GetProcAddress(hgldll, name);
	if (proc) return (void*)proc;
	return (void*)wglGetProcAddress(name);
}
