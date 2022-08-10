#pragma once
#include "core/core.h"

struct texture_t {

	pri u32 texture_id;
	pri i32 width_;
	pri i32 height_;

	pub void allocate(i32 width, i32 height, const r32* data) {
		width_ = width;
		height_ = height;
		gl.GenTextures(1, &texture_id);
		gl.BindTexture(gl.GL_TEXTURE_2D, texture_id);
		gl.TexImage2D(gl.GL_TEXTURE_2D, 0, gl.GL_RGB, width, height, 0, gl.GL_RGB, gl.GL_FLOAT, data);
	}

	pub void allocate(i32 width, i32 height, const u8* data) {
		width_ = width;
		height_ = height;
		gl.GenTextures(1, &texture_id);
		gl.BindTexture(gl.GL_TEXTURE_2D, texture_id);
		gl.TexImage2D(gl.GL_TEXTURE_2D, 0, gl.GL_RGB, width, height, 0, gl.GL_RGB, gl.GL_UNSIGNED_BYTE, data);
	}

	pub void update(const r32* data) {
		gl.TexSubImage2D(gl.GL_TEXTURE_2D, 0, 0, 0, width_, height_, gl.GL_RGB, gl.GL_FLOAT, data);
	}

	pub void repeat() {
		gl.BindTexture(gl.GL_TEXTURE_2D, texture_id);
		gl.TexParameteri(gl.GL_TEXTURE_2D, gl.GL_TEXTURE_WRAP_S, gl.GL_REPEAT);
		gl.TexParameteri(gl.GL_TEXTURE_2D, gl.GL_TEXTURE_WRAP_T, gl.GL_REPEAT);
	}

	pub void clamp_to_border() {
		gl.BindTexture(gl.GL_TEXTURE_2D, texture_id);
		gl.TexParameteri(gl.GL_TEXTURE_2D, gl.GL_TEXTURE_WRAP_S, gl.GL_CLAMP_TO_BORDER);
		gl.TexParameteri(gl.GL_TEXTURE_2D, gl.GL_TEXTURE_WRAP_T, gl.GL_CLAMP_TO_BORDER);
	}

	pub void clamp_to_edge() {
		gl.BindTexture(gl.GL_TEXTURE_2D, texture_id);
		gl.TexParameteri(gl.GL_TEXTURE_2D, gl.GL_TEXTURE_WRAP_S, gl.GL_CLAMP_TO_EDGE);
		gl.TexParameteri(gl.GL_TEXTURE_2D, gl.GL_TEXTURE_WRAP_T, gl.GL_CLAMP_TO_EDGE);
	}

	pub void no_filter() {
		gl.BindTexture(gl.GL_TEXTURE_2D, texture_id);
		gl.TexParameteri(gl.GL_TEXTURE_2D, gl.GL_TEXTURE_MAG_FILTER, gl.GL_NEAREST);
		gl.TexParameteri(gl.GL_TEXTURE_2D, gl.GL_TEXTURE_MIN_FILTER, gl.GL_NEAREST);
	}

	pub void filter() {
		gl.BindTexture(gl.GL_TEXTURE_2D, texture_id);
		gl.TexParameteri(gl.GL_TEXTURE_2D, gl.GL_TEXTURE_MAG_FILTER, gl.GL_LINEAR);
		gl.TexParameteri(gl.GL_TEXTURE_2D, gl.GL_TEXTURE_MIN_FILTER, gl.GL_LINEAR);
	}

	pub void use() {
		gl.BindTexture(gl.GL_TEXTURE_2D, texture_id);
	}
};
