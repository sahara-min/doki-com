#pragma once
#include "../shader.h"

st cexp cstring shader_crt_source = R"delim(

#define PI 3.1415926535897932384626433832795
#define VIEW vec2(240, 180)

#if defined(VERTEX)

attribute vec3 position;
attribute vec2 texcoord;
varying highp vec2 var_texcoord;

void main(void) {
	var_texcoord = texcoord; // * VIEW
	gl_Position = vec4(position, 1);
}

#elif defined(FRAGMENT)

varying highp vec2 var_texcoord;
uniform sampler2D signal;
uniform sampler2D mask;

void main(void) {
	vec2 vc = var_texcoord * VIEW;
	vec2 d = mod(vc - vec2(0.5, 0), 1);
	vec4 c1 = texture2D(signal, (vc - vec2(0.5, 0)) / VIEW);
	vec4 c2 = texture2D(signal, (vc + vec2(0.5, 0)) / VIEW);
	vec4 c3 = mix(c1, c2, d.x);
	gl_FragColor = c3 * sin(3.1415 * d.y);// texture2D(mask, vc * vec2(1, 1.5));
}

#endif

)delim";

struct shader_crt_t : shader_t {

	i32 signal_loc;
	i32 mask_loc;

	pub void compile() {
		shader_t::compile(shader_crt_source);
		signal_loc = gl.GetUniformLocation(program_id, "signal");
		mask_loc = gl.GetUniformLocation(program_id, "mask");
	}

	pub void use() {
		shader_t::use();
		gl.Uniform1i(signal_loc, 0);
		gl.Uniform1i(mask_loc, 1);
	}
};

st shader_crt_t shader_crt;
