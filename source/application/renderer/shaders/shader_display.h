#pragma once
#include "../shader.h"

st cexp cstring shader_display_source = R"delim(

#if defined(VERTEX)

attribute vec3 position;
attribute vec2 texcoord;
varying highp vec2 var_texcoord;

void main(void) {
	var_texcoord = texcoord;
	gl_Position = vec4(position * vec3(1, -1, 1), 1);
}

#elif defined(FRAGMENT)

varying highp vec2 var_texcoord;
uniform sampler2D sampler;

void main(void) {
	gl_FragColor = texture2D(sampler, var_texcoord);
}

#endif

)delim";

struct shader_display_t : shader_t {

	pub void compile() {
		shader_t::compile(shader_display_source);
	}

	pub void use() {
		shader_t::use();
	}
};

st shader_display_t shader_display;
