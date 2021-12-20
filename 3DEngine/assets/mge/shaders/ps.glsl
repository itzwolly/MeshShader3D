#version 450

// https://www.khronos.org/registry/OpenGL/extensions/NV/NV_fragment_shader_barycentric.txt
#extension GL_NV_fragment_shader_barycentric : enable

in PerVertexData {
  vec4 color;
} inColor;  

layout(location = 0) out vec4 outColor;

void main() {
	outColor = inColor.color;
}
    