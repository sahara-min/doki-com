#pragma once
#include "application/settings.h"

struct framebuffer_t {

	pri u32 texture_id;
	pri u32 buffer_id;
	pri i32 width_;
	pri i32 height_;

	pub void screen() {
		texture_id = 0;
		buffer_id = 0;
		width_ = settings::screen_scale * settings::screen_width;
		height_ = settings::screen_scale * settings::screen_height;
	}

	pub void allocate(i32 width, i32 height) {
		width_ = width;
		height_ = height;

		gl.GenTextures(1, &texture_id);
		gl.BindTexture(gl.GL_TEXTURE_2D, texture_id);
		gl.TexImage2D(gl.GL_TEXTURE_2D, 0, gl.GL_RGB, width, height, 0, gl.GL_RGB, gl.GL_UNSIGNED_BYTE, nullptr);

		gl.GenFramebuffers(1, &buffer_id);
		gl.BindFramebuffer(gl.GL_FRAMEBUFFER, buffer_id);
		gl.FramebufferTexture2D(gl.GL_FRAMEBUFFER, gl.GL_COLOR_ATTACHMENT0, gl.GL_TEXTURE_2D, texture_id, 0);
		u32 draw_buffers[1] = { gl.GL_COLOR_ATTACHMENT0 };
		gl.DrawBuffers(1, draw_buffers);
		gl.BindFramebuffer(gl.GL_FRAMEBUFFER, 0);
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

	pub void use_texture() {
		gl.BindTexture(gl.GL_TEXTURE_2D, texture_id);
	}

	pub void target() {
		gl.BindTexture(gl.GL_TEXTURE_2D, 0);
		gl.BindFramebuffer(gl.GL_FRAMEBUFFER, buffer_id);
		gl.Viewport(0, 0, width_, height_);
	}
};
