#version 330

in GeometryShaderOut
{
	vec2 uv;
	float active_passive;
	float leading;
} fs_in;

out vec4 color_out_of_fragment_shader;

float linearstep(float lo, float hi, float x)
{
	return (clamp(x, lo, hi) - lo) / (hi - lo);
}

void main()
{
	vec2 dist_vec = fs_in.uv - vec2(0.5f, 0.5f);
//    vec2 pos = gl_PointCoord - clipping_radius;
    float dist_squared = dot(dist_vec, dist_vec);
	float clipping_radius = 0.5f;
	
	if (dist_squared > clipping_radius * clipping_radius)
	{
		discard;
	}
	else
	{
		if (fs_in.active_passive > 0.5f)
		{
			if (fs_in.leading > 0.5f)
			{
				color_out_of_fragment_shader = vec4(0.33f, 0.0f, 0.0f, 1.0f);
			}
			else
			{
				color_out_of_fragment_shader = vec4(1.0f, 0.0f, 0.0f, 1.0f);
			}
		}
		else
		{
			if (fs_in.leading > 0.5f)
			{
				color_out_of_fragment_shader = vec4(0.0f, 0.33f, 0.33f, 1.0f);
			}
			else
			{
				color_out_of_fragment_shader = vec4(0.0f, 1.0f, 1.0f, 1.0f);
			}
		}
	}
}