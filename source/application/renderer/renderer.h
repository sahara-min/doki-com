#pragma once
#include "./shaders/shader_crt.h"
#include "./shaders/shader_display.h"
#include "./framebuffer.h"
#include "./shadow_mask_generator.h"
#include "./texture.h"
#include "application/computer/screen.h"
#include "application/settings.h"

struct renderer_t {

	pri st cexp r32 vertex_data[] = {
		-1.0f, -1.0f, 0.0f, /**/ 0.0f, 0.0f,
		1.0f, -1.0f, 0.0f, /**/ 1.0f, 0.0f,
		-1.0f, 1.0f, 0.0f, /**/ 0.0f, 1.0f,
		1.0f, 1.0f, 0.0f, /**/ 1.0f, 1.0f,
	};

	pri texture_t signal_texture;
	pri texture_t mask_texture;
	pri framebuffer_t target_buffer;
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
		shader_display.compile();

		signal_texture.allocate(settings::screen_width, settings::screen_height, (r32*)nullptr);
		signal_texture.no_filter();
		signal_texture.clamp_to_border();

		shadow_mask_generator.generate();

		mask_texture.allocate(shadow_mask_generator.width, shadow_mask_generator.height, shadow_mask_generator.pixels());
		mask_texture.filter();
		mask_texture.repeat();

		target_buffer.allocate(settings::target_scale * settings::screen_width, settings::target_scale * settings::screen_height);
		target_buffer.filter();
		target_buffer.clamp_to_edge();

		screen_buffer.screen();

		gl.Enable(gl.GL_TEXTURE_2D);
	}

	pub void update() {

		// pass 1
		target_buffer.target();
		shader_crt.use();
		gl.ActiveTexture(gl.GL_TEXTURE0);
		signal_texture.use();
		signal_texture.update(screen.image());
		gl.ActiveTexture(gl.GL_TEXTURE1);
		mask_texture.use();
		gl.DrawArrays(gl.GL_TRIANGLE_STRIP, 0, 4);
		
		// pass 2
		screen_buffer.target();
		shader_display.use();
		gl.ActiveTexture(gl.GL_TEXTURE0);
		target_buffer.use_texture();
		gl.DrawArrays(gl.GL_TRIANGLE_STRIP, 0, 4);
	}
};

st renderer_t renderer;
