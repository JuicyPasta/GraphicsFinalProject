#version 330 core
uniform sampler2D diffuseTexture;
uniform sampler2D specularTexture;
uniform float time;

out vec4 Outcolor;

in vec2 vTexCoord;
in vec3 L;
in vec3 E;
in vec3 N;


float fWidth = 648;
float fHeight = 324;

float texelSizeX = 1.0/fWidth;
float texelSizeY = 1.0/fHeight;

vec4 textureY( sampler2D textureSampler_i, vec2 texCoord_i ) {
    vec4 p0q0 = texture(textureSampler_i, texCoord_i);
    vec4 p1q0 = texture(textureSampler_i, texCoord_i + vec2(texelSizeX, 0));

    vec4 p0q1 = texture(textureSampler_i, texCoord_i + vec2(0, texelSizeY));
    vec4 p1q1 = texture(textureSampler_i, texCoord_i + vec2(texelSizeX , texelSizeY));

    float a = fract( texCoord_i.x * fWidth ); // Get Interpolation factor for X direction.
//					 Fraction near to valid data.

    vec4 pInterp_q0 = mix( p0q0, p1q0, a ); // Interpolates top row in X direction.
    vec4 pInterp_q1 = mix( p0q1, p1q1, a ); // Interpolates bottom row in X direction.

    float b = fract( texCoord_i.y * fHeight );// Get Interpolation factor for Y direction.
    return mix( pInterp_q0, pInterp_q1, b ); // Interpolate in Y direction.
//    return vec4(1,2, 2, 3);
}



//
vec4 cubic(float v)
{
    vec4 n = vec4(1.0, 2.0, 3.0, 4.0) - v;
    vec4 s = n * n * n;
    float x = s.x;
    float y = s.y - 4.0 * s.x;
    float z = s.z - 4.0 * s.y + 6.0 * s.x;
    float w = 6.0 - x - y - z;
    return vec4(x, y, z, w);
}
//
vec4 textureX(sampler2D tex, vec2 texcoord, vec2 texscale)
{
    float fx = fract(texcoord.x);
    float fy = fract(texcoord.y);
    texcoord.x -= fx;
    texcoord.y -= fy;

    vec4 xcubic = cubic(fx);
    vec4 ycubic = cubic(fy);

    vec4 c = vec4(texcoord.x - 0.5, texcoord.x + 1.5, texcoord.y - 0.5, texcoord.y + 1.5);
    vec4 s = vec4(xcubic.x + xcubic.y, xcubic.z + xcubic.w, ycubic.x + ycubic.y, ycubic.z + ycubic.w);
    vec4 offset = c + vec4(xcubic.y, xcubic.w, ycubic.y, ycubic.w) / s;

    vec4 sample0 = texture(tex, vec2(offset.x, offset.z) * texscale);
    vec4 sample1 = texture(tex, vec2(offset.y, offset.z) * texscale);
    vec4 sample2 = texture(tex, vec2(offset.x, offset.w) * texscale);
    vec4 sample3 = texture(tex, vec2(offset.y, offset.w) * texscale);

    float sx = s.x / (s.x + s.y);
    float sy = s.z / (s.z + s.w);

    return mix(
        mix(sample3, sample2, sx),
        mix(sample1, sample0, sx), sy);
}
 float Triangular( float f )
{
	f = f / 2.0;
	if( f < 0.0 )
	{
		return ( f + 1.0 );
	}
	else
	{
		return ( 1.0 - f );
	}
	return 0.0;
}


vec4 BiCubic( sampler2D textureSampler, vec2 TexCoord )
{
    float texelSizeX = 1.0 / fWidth; //size of one texel
    float texelSizeY = 1.0 / fHeight; //size of one texel
    vec4 nSum = vec4( 0.0, 0.0, 0.0, 0.0 );
    vec4 nDenom = vec4( 0.0, 0.0, 0.0, 0.0 );
    float a = fract( TexCoord.x * fWidth ); // get the decimal part
    float b = fract( TexCoord.y * fHeight ); // get the decimal part
    for( int m = -1; m <=2; m++ )
    {
        for( int n =-1; n<= 2; n++)
        {
			vec4 vecData = texture(textureSampler,
                               TexCoord + vec2(texelSizeX * float( m ),
					texelSizeY * float( n )));
			float f  = Triangular( float( m ) - a );
			vec4 vecCooef1 = vec4( f,f,f,f );
			float f1 = Triangular ( -( float( n ) - b ) );
			vec4 vecCoeef2 = vec4( f1, f1, f1, f1 );
            nSum = nSum + ( vecData * vecCoeef2 * vecCooef1  );
            nDenom = nDenom + (( vecCoeef2 * vecCooef1 ));
        }
    }
    return nSum / nDenom;
}

void main() {
//  vec4 texColor0 = textureX(diffuseTexture, vTexCoord, vec2(1,1));
  vec4 texColor0 = BiCubic(diffuseTexture, vTexCoord);
//  vec4 specularTextureColor = texture(specularTexture, vTexCoord);
    vec4 specularTextureColor = vec4(1, 1, 1, 1);

  float shiny = 10;
  float ambient = .3;

  vec3 L_norm = normalize(L);
  vec3 N_norm = normalize(N);
  vec3 E_norm = normalize(E);
  vec3 R_norm = normalize(-reflect(L_norm,N_norm));

  vec3 diffL = texColor0.xyz * max(dot(N_norm, L_norm), 0.0);
  diffL = clamp(diffL, 0.0, 1.0);

  vec3 specL = specularTextureColor.xyz * pow(max(dot(R_norm, E_norm), 0.0), shiny);
  specL = clamp(specL, 0.0, 1.0);

  Outcolor = vec4(diffL + specL + ambient*texColor0.xyz, 1);
  Outcolor.a = 1 - time*time;
}
