#pragma once

#ifdef NDEBUG
#define assert(cond)
#else
#define assert(cond) if (!cond) throw 0;
#endif

#define st static
#define cexp constexpr
#define ceva consteval
#define pub public:
#define pro protected:
#define pri private:

using i8 = signed char;
using u8 = unsigned char;
using i16 = signed short;
using u16 = unsigned short;
using i32 = signed long;
using u32 = unsigned long;
using r32 = float;
using cstring = const char*;

// ---------------------------------------------------------------------------
// Window

struct window_t {

	pri i32 width_;
	pri i32 height_;

	pub void set_title(cstring title) {
		void os_set_window_title(cstring title);
		os_set_window_title(title);
	}

	pub void set_size(i32 width, i32 height) {
		void os_set_window_size(i32 width, i32 height);
		os_set_window_size(width, height);
		width_ = width;
		height_ = height;
	}

	pub i32 width() {
		return width_;
	}

	pub i32 height() {
		return height_;
	}
};

st window_t window;

// ---------------------------------------------------------------------------
// Menu

struct menu_t {

	pub void add_item(i32 id, cstring string) {
		void os_add_menu_item(i32 id, cstring string);
		os_add_menu_item(id, string);
	}

	pub void add_separator() {
		void os_add_menu_separator();
		os_add_menu_separator();
	}

	pub i32 get_result() {
		long os_get_menu_result();
		return os_get_menu_result();
	}
};

st menu_t menu;

// ---------------------------------------------------------------------------
// Config

struct config_t {

	pub i32 read(cstring key, i32 default_value) {
		long os_read_config(const char* key, long default_value);
		return os_read_config(key, default_value);
	}

	pub void write(cstring key, i32 value) {
		void os_write_config(const char* key, long value);
		os_write_config(key, value);
	}
};

st config_t config;

// ---------------------------------------------------------------------------
// File

struct file_t {

	pub bool dropped() {
		bool os_file_dropped();
		return os_file_dropped();
	}

	pub bool read(void* buffer, i32 count) {
		bool os_file_read(void* buffer, i32 count);
		return os_file_read(buffer, count);
	}

	pub bool is_ready() {
		bool os_file_is_ready();
		return os_file_is_ready();
	}
};

st file_t file;

// ---------------------------------------------------------------------------
// OpenGL

#pragma push_macro("STDCALL")
#pragma push_macro("GF")
#ifdef _WIN32
#define STDCALL __stdcall
#else
#define STDCALL
#endif
#define GF(rtype, fname, ...) pri cstring fname##_name = "gl"#fname; pub rtype (STDCALL* fname) (__VA_ARGS__)

struct gl_t {

	pub enum {
		GL_TRIANGLES = 0x0004,
		GL_TRIANGLE_STRIP = 0x0005,
		GL_GREATER = 0x0204,
		GL_ALPHA_TEST = 0x0BC0,
		GL_SCISSOR_TEST = 0x0C11,
		GL_TEXTURE_2D = 0x0DE1,
		GL_UNSIGNED_BYTE = 0x1401,
		GL_FLOAT = 0x1406,
		GL_RGB = 0x1907,
		GL_RGBA = 0x1908,
		GL_NEAREST = 0x2600,
		GL_LINEAR = 0x2601,
		GL_TEXTURE_MAG_FILTER = 0x2800,
		GL_TEXTURE_MIN_FILTER = 0x2801,
		GL_TEXTURE_WRAP_S = 0x2802,
		GL_TEXTURE_WRAP_T = 0x2803,
		GL_REPEAT = 0x2901,
		GL_COLOR_BUFFER_BIT = 0x4000,
		GL_CLAMP_TO_BORDER = 0x812D,
		GL_CLAMP_TO_EDGE = 0x812F,
		GL_TEXTURE0 = 0x84C0,
		GL_TEXTURE1 = 0x84C1,
		GL_ARRAY_BUFFER = 0x8892,
		GL_DYNAMIC_DRAW = 0x88E8,
		GL_FRAGMENT_SHADER = 0x8B30,
		GL_VERTEX_SHADER = 0x8B31,
		GL_COLOR_ATTACHMENT0 = 0x8CE0,
		GL_FRAMEBUFFER = 0x8D40,
	};

	GF(void, ActiveTexture, u32 texture);
	GF(void, AlphaFunc, u32 func, r32 reference);
	GF(void, AttachShader, u32 program, u32 shader);
	GF(void, BindBuffer, u32 target, u32 buffer);
	GF(void, BindFramebuffer, u32 target, u32 framebuffer);
	GF(void, BindTexture, u32 target, u32 texture);
	GF(void, BindVertexArray, u32 array);
	GF(void, BufferData, u32 target, i32 size, const void* data, u32 usage);
	GF(void, BufferSubData, u32 target, i32 offset, i32 size, const void* data);
	GF(void, Clear, u32 mask);
	GF(void, ClearColor, r32 red, r32 green, r32 blue, r32 alpha);
	GF(void, CompileShader, u32 shader);
	GF(u32, CreateProgram);
	GF(u32, CreateShader, u32 type);
	GF(void, DeleteTextures, i32 n, u32* textures);
	GF(void, Disable, u32 cap);
	GF(void, DrawArrays, u32 mode, i32 first, i32 count);
	GF(void, DrawBuffers, i32 n, u32* bufs);
	GF(void, Enable, u32 cap);
	GF(void, EnableVertexAttribArray, u32 index);
	GF(void, FramebufferTexture2D, u32 target, u32 attachment, u32 textarget, u32 texture, u32 level);
	GF(void, GenBuffers, i32 n, u32* buffers);
	GF(void, GenFramebuffers, i32 n, u32* ids);
	GF(void, GenTextures, i32 n, u32* textures);
	GF(void, GenVertexArrays, i32 n, u32* arrays);
	GF(i32, GetAttribLocation, u32 program, cstring name);
	GF(u32, GetError);
	GF(void, GetProgramInfoLog, u32 program, i32 bufSize, i32* length, char* infoLog);
	GF(void, GetShaderInfoLog, u32 shader, i32 bufSize, i32* length, char* infoLog);
	GF(i32, GetUniformLocation, u32 program, cstring name);
	GF(void, LinkProgram, u32 program);
	GF(void, Scissor, i32 x, i32 y, i32 width, i32 height);
	GF(void, ShaderSource, u32 shader, i32 count, cstring* string, const i32* length);
	GF(void, TexImage2D, u32 target, i32 level, i32 internalformat, i32 width, i32 height, i32 border, u32 format, u32 type, const void* pixels);
	GF(void, TexParameteri, u32 target, u32 pname, i32 param);
	GF(void, TexSubImage2D, u32 target, i32 level, i32 x, i32 y, i32 width, i32 height, u32 format, u32 type, const void* pixels);
	GF(void, Uniform1f, i32 location, r32 v0);
	GF(void, Uniform1i, i32 location, i32 v0);
	GF(void, Uniform2f, i32 location, r32 v0, r32 v1);
	GF(void, UniformMatrix4fv, i32 location, i32 count, bool transpose, const r32* value);
	GF(void, UseProgram, u32 program);
	GF(void, VertexAttribPointer, u32 index, i32 size, u32 type, bool normalized, i32 stride, const void* pointer);
	GF(void, Viewport, i32 x, i32 y, i32 width, i32 height);

	pri struct entry_t { cstring name; void* proc; };

	pub void init(i32 major_version, i32 minor_version) {
		void os_init_gl(i32 major_version, i32 minor_version);
		void* os_get_gl_proc(const char* name);
		os_init_gl(major_version, minor_version);
		entry_t* entries = (entry_t*)this;
		for (i32 i = 0; i < sizeof(gl_t) / sizeof(entry_t); i++) {
			entry_t* entry = &entries[i];
			entry->proc = os_get_gl_proc(entry->name);
		}
	}
};

st gl_t gl;

#pragma pop_macro("STDCALL")
#pragma pop_macro("GF")

// ---------------------------------------------------------------------------
// Core

struct core_t {

	void init() {
	}
};

st core_t core;
