#version 460 core


out vec4 FragColor;

in VS_OUT
{
	vec3 FragPos;
	vec3 Normal;
	noperspective vec2 TexCoords;
	mat3 TBN;
} fs_in;

//uniform samplerCubeShadow depthMap;
uniform sampler2D texture_diffuse1;
uniform sampler2D texture_normal1;
uniform sampler2D texture_specular1;

uniform vec3 lightPos;
uniform vec3 viewPos;
//uniform bool shadows;
//uniform float far_plane;

/*float ShadowCalculation(vec3 fragPos, float bias)
{
    vec3 fragToLight = fragPos - lightPos;
	
    float currentDepth = length(fragToLight) / far_plane;
 
    //float shadow = texture(depthMap, vec4(fragToLight, currentDepth - bias));
    
	float shadow  = 0.0;
	float samples = 4.0;
	float offset  = 0.1;
	for(float x = -offset; x < offset; x += offset / (samples * 0.5))
	{
		for(float y = -offset; y < offset; y += offset / (samples * 0.5))
		{
			for(float z = -offset; z < offset; z += offset / (samples * 0.5))
			{
				float temp_shadow = texture(depthMap, vec4(fragToLight + vec3(x,y,z), currentDepth - bias));
				shadow += temp_shadow;
			}
		}
	}
	shadow /= (samples * samples * samples);
        
    return shadow;
}
*/

void main()
{
	vec3 color = texture(texture_diffuse1, fs_in.TexCoords).rgb;
	vec3 normal = texture(texture_normal1, fs_in.TexCoords).rgb;
	normal = normalize(normal * 2.0f - 1.0f);
	normal = normalize(fs_in.TBN * normal);
	//vec3 normal = fs_in.Normal;
	//normal = vec3(0.0f,0.0f,1.0f);
	// ambient
	vec3 ambient = 0.1f * color;
	// diffuse
	vec3 lightDir = normalize(lightPos - fs_in.FragPos);
	float diff = max(dot(lightDir, normal), 0.0f);
	vec3 diffuse = diff * color;
	// specular
	vec3 viewDir = normalize(viewPos - fs_in.FragPos);
	float spec = 0.0f;
	// blinnFongLighting
	vec3 halfWayDir = normalize(lightDir + viewDir);
	spec = pow(max(dot(halfWayDir, normal), 0.0f), 64.0f);

	vec3 specular = texture(texture_specular1, fs_in.TexCoords).rgb * spec;
	//float bias = max(0.05f * (1.0f - dot(fs_in.Normal,lightDir)),0.005f);
	//float shadow = shadows ? ShadowCalculation(fs_in.FragPos,bias) : 1.0f;
	//FragColor = vec4(ambient + shadow  * (diffuse + specular), 1.0f);
	FragColor = vec4(ambient + diffuse + specular, 1.0f);


}