#version 330

in vec2 position;
in vec2 direction;
//in float cluster_color;//CLUSTER_COLORING
in float color_r;
in float color_g;
in float color_b;
in float active_passive;

uniform float L;
uniform float lambda;
uniform float l;
uniform int n;

uniform float x_shift;
uniform float y_shift;
uniform float z_scale;

out VertexShaderOut
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
} vs_out;

void main()
{
	mat4 translate = mat4(1.0);
	translate[3] = vec4(-1.0, -1.0, 0.0, 1.0);
	
	mat4 scale = mat4(1.0);
    scale[0][0] = 2.0f / L;
    scale[1][1] = 2.0f / L;
    
    mat4 external_translate = mat4(1.0);
    external_translate[3] = vec4(x_shift, y_shift, 0.0f, 1.0f);
	mat4 external_scale = mat4(1.0);
	external_scale[0][0] = z_scale;
	external_scale[1][1] = z_scale;

	gl_Position = external_translate * external_scale * translate * scale * vec4(position, 0.0f, 1.0f);
//	gl_PointSize = 10.0f;

    vs_out.lambda = lambda * 2.0f / L;
    vs_out.l = l * 2.0f / L;
	vs_out.n = n;
	vs_out.active_passive = active_passive;
	vs_out.z_scale = z_scale;
//	vs_out.cluster_color = cluster_color;//CLUSTER_COLORING
    vs_out.color_r = color_r;
    vs_out.color_g = color_g;
    vs_out.color_b = color_b;

    vs_out.direction = normalize(direction);
    if (isnan(vs_out.direction.x) || isnan(vs_out.direction.y))
    {
        vs_out.direction.x = sqrt(2.0f) / 2.0f;
        vs_out.direction.y = sqrt(2.0f) / 2.0f;
    	vs_out.active_passive = 0.0f;
    	vs_out.color_r = 0.0f;
        vs_out.color_g = 0.0f;
        vs_out.color_b = 0.0f;
        vs_out.l = lambda * 2.0f / L;
        vs_out.n = 1;
    }
}