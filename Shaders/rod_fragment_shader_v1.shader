#version 330

in GeometryShaderOut
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
} fs_in;

out vec4 color_out_of_fragment_shader;

float linearstep(float lo, float hi, float x)
{
	return (clamp(x, lo, hi) - lo) / (hi - lo);
}

vec3 Jet2Rgb(float j)
{
	float r, g, b;
	
	if (j <= 1.0f / 8.0f)
	{
		r = 0.0f;
		g = 0.0f;
		b = (1.0f - 9.0f / 16.0f) / (1.0f / 8.0f - 0.0f) * (j - 0.0f) + 9.0f / 16.0f;
	}
	else if (j <= 3.0f / 8.0f)
	{
		r = 0.0f;
		g = (1.0f - 0.0f) / (3.0f / 8.0f - 1.0f / 8.0f) * (j - 1.0f / 8.0f) + 0.0f;
		b = 1.0f;
	}
	else if (j <= 5.0f / 8.0f)
	{
		r = (1.0f - 0.0f) / (5.0f / 8.0f - 3.0f / 8.0f) * (j - 3.0f / 8.0f) + 0.0f;
		g = 1.0f;
		b = (0.0f - 1.0f) / (5.0f / 8.0f - 3.0f / 8.0f) * (j - 3.0f / 8.0f) + 1.0f;
	}
	else if (j <= 7.0f / 8.0f)
	{
		r = 1.0f;
		g = (0.0f - 1.0f) / (7.0f / 8.0f - 5.0f / 8.0f) * (j - 5.0f / 8.0f) + 1.0f;
		b = 0.0f;
	}
	else
	{
		r = (9.0f / 16.0f - 1.0f) / (1.0f - 7.0f / 8.0f) * (j - 7.0f / 8.0f) + 1.0f;
		g = 0.0f;
		b = 0.0f;
	}
	
	vec3 color;
	color.x = r;
	color.y = g;
	color.z = b;
	return color;
}

void main()
{
	if ((fs_in.uv.x * fs_in.uv.x) / (fs_in.greater_half_axis * fs_in.greater_half_axis) + (fs_in.uv.y * fs_in.uv.y) / (fs_in.smaller_half_axis * fs_in.smaller_half_axis) > 1.0f)
	{
		discard;
	}
	else
	{
		if (fs_in.active_passive > 0.5f)
		{
			if (fs_in.uv.x > fs_in.focus / 2.0f)
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
			if (fs_in.uv.x > fs_in.focus / 2.0f)
			{
				color_out_of_fragment_shader = vec4(0.0f, 0.33f, 0.33f, 1.0f);
			}
			else
			{
				color_out_of_fragment_shader = vec4(0.0f, 1.0f, 1.0f, 1.0f);
			}
		}
//		color_out_of_fragment_shader = vec4(Jet2Rgb(fs_in.cluster_color), 1.0f);//CLUSTER_COLORING
//        color_out_of_fragment_shader = vec4(fs_in.color_r, fs_in.color_g, fs_in.color_b, 1.0f);
	}
}