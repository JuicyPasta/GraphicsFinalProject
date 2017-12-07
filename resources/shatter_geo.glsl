#version 330 core
layout(triangles) in;
layout(triangle_strip, max_vertices=3) out;

uniform float time;
uniform mat4 V;
uniform mat4 P;

in vec3 vertRaw[];
in vec3 vL[3];
in vec3 vE[3];
in vec3 vN[3];
in vec2 vvTexCoord[3];
in vec4 vShadowCoord[3];

out vec4 shadowCoord;
out vec2 vTexCoord;
out vec3 L;
out vec3 E;
out vec3 N;

float adj_time = clamp(time,0,1);

/* From https://www.shadertoy.com/view/Xd23Dh */
vec3 hash3( vec2 p )
{

    vec3 q = vec3( dot(p,vec2(127.1,311.7)),
                  dot(p,vec2(269.5,183.3)),
                  dot(p,vec2(419.2,371.9)) );
    return fract(sin(q)*43758.5453) - .5;
}

mat4 shatterTranslate(float seed) {
//    return mat4(vec4(1,0,0,0),vec4(0,1,0,0),vec4(0,0,1,0),vec4(0,0,0,1));

    vec3 v = hash3(vec2(seed,seed + .1));
    vec3 a = hash3(vec2(seed + .4,seed + .5));
    v.y = -.3;
    a.y = -.3;
    vec3 x = time*v + .5 * adj_time * adj_time * a; // delta = vt + 1/2at^2
    x /= 1000.0;
    return mat4(vec4(1,0,0,x.x),vec4(0,1,0,x.y),vec4(0,0,1,x.z),vec4(0,0,0,1));
}

/* Rotate about x-axis from shadertoy.com/view/llXXz4 */
mat4 rotX(float t) {
    float cx = cos(t), sx = sin(t);
    return mat4(1, 0, 0, 0,
                0, cx, sx, 0,
                0, -sx, cx, 0,
                0,0,0,1);
}


/* Rotate about y-axis from shadertoy.com/view/llXXz4 */
mat4 rotY(float t) {
    float cy = cos(t), sy = sin(t);
    return mat4(cy, 0, -sy, 0,
                0, 1., 0, 0,
                sy, 0, cy, 0,
                0,0,0,1);

}

mat4 shatterRotate(float seed) {
    vec3 r = hash3(vec2(seed + .25,seed + .75));
    r *= adj_time/10;
    return rotX(r.x)*rotY(r.y);
}

mat4 shiftOut(vec3 norm) {
    float speed = 200./adj_time;
    return mat4(vec4(1,0,0,norm.x/speed),vec4(0,1,0,norm.y/speed),vec4(0,0,1,norm.z/speed),vec4(0,0,0,1));
}

void main()
{
  float seed = vertRaw[1].x * .6
             + vertRaw[1].y
             + vertRaw[1].z * .7;
  vec3 norm = vertRaw[0] + vertRaw[1] + vertRaw[2] - vec3(.5,.5,0);
  vec4 avg = vec4((gl_in[0].gl_Position + gl_in[1].gl_Position + gl_in[2].gl_Position)/3);
  avg.a = 1;
  for(int i=0; i<3; i++)
  {
    vTexCoord = vvTexCoord[i];
    L = vL[i];
    E = vE[i];
    N = vN[i];
    shadowCoord = vShadowCoord[i];

    vec4 adj_pos = gl_in[i].gl_Position * (1 - adj_time) + avg * adj_time;
    gl_Position = P * V * shatterTranslate(seed) * shatterRotate(seed) * adj_pos;
    EmitVertex();
  }
  EndPrimitive();
}