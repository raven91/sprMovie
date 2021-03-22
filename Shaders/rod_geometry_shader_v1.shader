#version 330

layout (points) in;
layout (triangle_strip, max_vertices = 4) out;

in VertexShaderOut
{
    vec2 direction;
    float lambda;
    float l;
    int n;
//	float cluster_color;//CLUSTER_COLORING
    float color_r;
    float color_g;
    float color_b;
	float active_passive;
	float z_scale;
} gs_in[1];

out GeometryShaderOut
{
	vec2 uv;
	float greater_half_axis;
	float smaller_half_axis;
	float focus;
//	float cluster_color;//CLUSTER_COLORING
    float color_r;
    float color_g;
    float color_b;
	float active_passive;
	vec2 direction;
} gs_out;

void main()
{
	vec3 r = gl_in[0].gl_Position.xyz;
    vec3 u_par = vec3(gs_in[0].direction, 0.0f);
	vec3 u_orth = vec3(-u_par.y, u_par.x, 0.0f);

	float greater_half_axis, smaller_half_axis, focus;
	greater_half_axis = gs_in[0].l / 2.0f * gs_in[0].z_scale;
	smaller_half_axis = gs_in[0].lambda / 2.0f * gs_in[0].z_scale;
	focus = sqrt(greater_half_axis * greater_half_axis - smaller_half_axis * smaller_half_axis);
	
	gs_out.greater_half_axis = greater_half_axis;
	gs_out.smaller_half_axis = smaller_half_axis;
	gs_out.focus = focus;
	
	gs_out.active_passive = gs_in[0].active_passive;
//	gs_out.cluster_color = gs_in[0].cluster_color;//CLUSTER_COLORING
    gs_out.color_r = gs_in[0].color_r;
    gs_out.color_g = gs_in[0].color_g;
    gs_out.color_b = gs_in[0].color_b;
    gs_out.direction = gs_in[0].direction;

	vec3 A, B, C, D;
	A = r - greater_half_axis * u_par - smaller_half_axis * u_orth;
	B = r + greater_half_axis * u_par - smaller_half_axis * u_orth;
	C = r + greater_half_axis * u_par + smaller_half_axis * u_orth;
	D = r - greater_half_axis * u_par + smaller_half_axis * u_orth;
	
	gs_out.uv = vec2(greater_half_axis, -smaller_half_axis); gl_Position = vec4(B, 1.0f); EmitVertex();
	gs_out.uv = vec2(greater_half_axis, smaller_half_axis); gl_Position = vec4(C, 1.0f); EmitVertex();
	gs_out.uv = vec2(-greater_half_axis, -smaller_half_axis); gl_Position = vec4(A, 1.0f); EmitVertex();
	gs_out.uv = vec2(-greater_half_axis, smaller_half_axis); gl_Position = vec4(D, 1.0f); EmitVertex();
	
	EndPrimitive();
}