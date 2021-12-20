#version 450
#extension GL_NV_mesh_shader : require

// Max setting for mesh shaders:
// 		=> local_size_x = 32
// Set the number of threads per workgroup (always one-dimensional).
layout(local_size_x = 32) in;

// Max settings for RTX 2080TI: 
// 		=> max_vertices = 256
//		=> max_primitives = 512
// The primitive type (points, lines or triangles)
layout(points, max_vertices = 256, max_primitives = 256) out;
 
layout (std430, binding = 1) buffer _vertices {
	vec4 vertices[];
} vbPosition;

layout (std430, binding = 2) buffer _colors {
	vec4 vertices[];
} vbColor;

out PerVertexData {
  vec4 color;
} v_out[];  // max_vertices 

uniform	mat4 	projectionMatrix;
uniform	mat4 	viewMatrix;
uniform	mat4 	modelMatrix;

void main() {
	uint local_thread_id = gl_LocalInvocationID.x;
	uint workgroup_id = gl_WorkGroupID.x;
	
	// Amount of primitives (points) to process per workgroup.
	uint primAmount = 256; 
	// Amount of threads per workgroup, which is the same as: local_size_x
	uint groupSize = gl_WorkGroupSize.x; 
	// Amount of loops required with the amount of available threads
	uint primLoops = max(1, primAmount / groupSize); 
	
	// Example: with primAmount equals 128 you'll have (128 / 32) = 4 loops
	for (int i = 0; i < primLoops; i++) {
		uint localVertexIndex = local_thread_id + i * groupSize;
		uint globalVertexIndex = localVertexIndex + (workgroup_id * (primAmount + 1));
		
		// Position Vertices
		gl_MeshVerticesNV[localVertexIndex].gl_Position = projectionMatrix * viewMatrix * modelMatrix * vbPosition.vertices[globalVertexIndex];
		
		// Color Vertices
		v_out[localVertexIndex].color = vbColor.vertices[globalVertexIndex];
		
		// Primitive Indices
		gl_PrimitiveIndicesNV[localVertexIndex] = localVertexIndex;
	}
	// Amount of primitives (points) to output per workgroup
	gl_PrimitiveCountNV = primAmount; 
}