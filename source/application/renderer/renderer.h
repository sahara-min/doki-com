#pragma once
#include "./shaders/shader_crt.h"
#include "./framebuffer.h"
#include "./texture.h"
#include "application/computer/screen.h"
#include "application/constants.h"

struct renderer_t {

	pri st cexp r32 vertex_data[] = {
		-1.0f, 1.0f,
		1.0f, 1.0f, 
		-1.0f, -1.0f,
		1.0f, -1.0f,
	};

	pri texture_t signal_texture;
	pri framebuffer_t screen_buffer;

	pub void init() {

		gl.init(3, 1);

		u32 vao;
		gl.GenVertexArrays(1, &vao);
		gl.BindVertexArray(vao);

		u32 vertex_buffer;
		gl.GenBuffers(1, &vertex_buffer);
		gl.BindBuffer(gl.GL_ARRAY_BUFFER, vertex_buffer);
		gl.BufferData(gl.GL_ARRAY_BUFFER, sizeof(vertex_data), vertex_data, gl.GL_DYNAMIC_DRAW);

		shader_crt.compile();

		signal_texture.allocate(constants::screen_width, constants::screen_height, (r32*)nullptr);
		signal_texture.no_filter();
		signal_texture.clamp_to_border();

		screen_buffer.screen();
	}

	pub void update() {
		screen_buffer.target();
		shader_crt.use();
		signal_texture.use();
		signal_texture.update(screen.image());
		gl.DrawArrays(gl.GL_TRIANGLE_STRIP, 0, 4);
	}
};

st renderer_t renderer;
