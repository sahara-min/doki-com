#pragma once
#include "../shader.h"

st cexp cstring shader_hblur_source = R"delim(

#if defined(VERTEX)

attribute vec3 position;
attribute vec2 texcoord;
varying highp vec2 var_texcoord;
uniform vec2 in_size;

void main(void) {
	var_texcoord = in_size * texcoord;
	gl_Position = vec4(position, 1);
}

#elif defined(FRAGMENT)

varying highp vec2 var_texcoord;
uniform sampler2D signal;
uniform vec2 in_size;

void main(void) {
	
	gl_FragColor = texture2D(sampler, var_texcoord / in_size);
}

#endif

)delim";

struct shader_hblur_t : shader_t {

	pub void compile() {
		shader_t::compile(shader_hblur_source);
	}

	pub void use() {
		shader_t::use();
	}
};

st shader_hblur_t shader_hblur;
