#pragma once
#include "core/core.h"

struct shader_t {

	pro u32 program_id;
	pro i32 position_loc;
	pro i32 texcoord_loc;

	pub void compile(cstring source) {

		st char log_buffer[1024];

		program_id = 0;

		u32 vs = gl.CreateShader(gl.GL_VERTEX_SHADER);
		cstring vsources[] = { "#version 140\n#define VERTEX\n", source };
		gl.ShaderSource(vs, 2, vsources, nullptr);
		gl.CompileShader(vs);
		gl.GetShaderInfoLog(vs, sizeof(log_buffer), nullptr, log_buffer);
		if (log_buffer[0] != '\0')
			return;

		u32 fs = gl.CreateShader(gl.GL_FRAGMENT_SHADER);
		cstring fsources[] = { "#version 140\n#define FRAGMENT\n", source };
		gl.ShaderSource(fs, 2, fsources, nullptr);
		gl.CompileShader(fs);
		gl.GetShaderInfoLog(fs, sizeof(log_buffer), nullptr, log_buffer);
		if (log_buffer[0] != '\0')
			return;

		u32 p = gl.CreateProgram();
		gl.AttachShader(p, vs);
		gl.AttachShader(p, fs);
		gl.LinkProgram(p);
		gl.GetProgramInfoLog(p, sizeof(log_buffer), nullptr, log_buffer);
		if (log_buffer[0] != '\0')
			return;

		position_loc = gl.GetAttribLocation(p, "position");
		texcoord_loc = gl.GetAttribLocation(p, "texcoord");

		program_id = p;
	}

	pub void use() {
		gl.UseProgram(program_id);
		gl.VertexAttribPointer(position_loc, 3, gl.GL_FLOAT, false, 20, (void*)0);
		gl.VertexAttribPointer(texcoord_loc, 2, gl.GL_FLOAT, false, 20, (void*)12);
		gl.EnableVertexAttribArray(position_loc);
		gl.EnableVertexAttribArray(texcoord_loc);
	}
};
