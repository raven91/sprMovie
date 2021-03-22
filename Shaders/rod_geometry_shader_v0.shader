#version 330

//uniform int sprn;

layout (points) in;
layout (triangle_strip, max_vertices = 12) out;

in VertexShaderOut
{
	float direction;
	float lambda;
	float active_passive;
	int n;
} gs_in[1];

out GeometryShaderOut
{
	vec2 uv;
	float active_passive;
	float leading;
} gs_out;

void main()
{
	vec3 r = gl_in[0].gl_Position.xyz;
	vec3 u = vec3(cos(gs_in[0].direction), sin(gs_in[0].direction), 0.0f);
	vec3 u_orth = vec3(-u.y, u.x, 0.0f);

	gs_out.active_passive = gs_in[0].active_passive;

	vec3 A, B, C, D;
	vec3 r_bead;
	//number of beads is odd
	if (bool(gs_in[0].n % 2))
	{
		for (int i = -(gs_in[0].n / 2); i <= gs_in[0].n / 2; ++i)
		{
			if (i == gs_in[0].n / 2)
			{
				gs_out.leading = 1.0f;
			}
			else
			{
				gs_out.leading = 0.0f;
			}
			
			r_bead = r + u * i * gs_in[0].lambda;
			
			A = r_bead + gs_in[0].lambda / 2.0f * (- u - u_orth);
			B = r_bead + gs_in[0].lambda / 2.0f * (+ u - u_orth);
			C = r_bead + gs_in[0].lambda / 2.0f * (+ u + u_orth);
			D = r_bead + gs_in[0].lambda / 2.0f * (- u + u_orth);
			
			gs_out.uv = vec2(1, 0); gl_Position = vec4(B, 1.0f); EmitVertex();
			gs_out.uv = vec2(1, 1); gl_Position = vec4(C, 1.0f); EmitVertex();
			gs_out.uv = vec2(0, 0); gl_Position = vec4(A, 1.0f); EmitVertex();
			gs_out.uv = vec2(0, 1); gl_Position = vec4(D, 1.0f); EmitVertex();
			
			EndPrimitive();
		}
	}
	//number of beads is even
	else
	{

	}
}