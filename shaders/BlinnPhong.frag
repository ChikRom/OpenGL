#version 460 core


out vec4 FragColor;

in VS_OUT
{
	vec3 FragPos;
	vec3 Normal;
	vec2 TexCoords;
	vec4 FragPosLightSpace;
} fs_in;

uniform sampler2D depthMap;
uniform sampler2D woodTexture;

//uniform vec3 lightPos;
//uniform vec3 viewPos;
uniform bool blinn;
uniform bool kindOfLight;

struct SpotLight
{
	vec3 position;
	vec3 direction;
	float cutOff;
	float outerCutOff;
};

struct DirectionLight
{
	vec3 position;
};

uniform SpotLight spot_light;
uniform DirectionLight direction_light;

float ShadowCalculate(vec4 fragPosLightSpace, float bias)
{
	vec3 ndcPos = fragPosLightSpace.xyz / fragPosLightSpace.w;
	// transform to [0,1] range
	ndcPos = ndcPos * 0.5f + 0.5f;

	float currentDepth = ndcPos.z;
	float shadow = 0.0;
	vec2 texelSize = 1.0 / textureSize(depthMap, 0);
	for(int x = -1; x <= 1; ++x)
	{
    for(int y = -1; y <= 1; ++y)
		{
			float pcfDepth = texture(depthMap, ndcPos.xy + vec2(x, y) * texelSize).r; 
			shadow += currentDepth - bias > pcfDepth ? 1.0 : 0.0;        
		}    
	}
	shadow /= 9.0;
	return shadow;
}

void main()
{
	vec3 viewPos = spot_light.position;
	vec3 color = texture(woodTexture, fs_in.TexCoords).rgb;
	// ambient
	vec3 ambient = 0.1f * color;
	// diffuse
	vec3 lightPos;
	if (!kindOfLight)
	{
		lightPos = direction_light.position;
	}
	else
	{
		lightPos = spot_light.position;
	}
	vec3 lightDir = normalize(lightPos - fs_in.FragPos);
	float diff = max(dot(lightDir, fs_in.Normal), 0.0f);
	vec3 diffuse = diff * color;
	// specular
	vec3 viewDir = normalize(viewPos - fs_in.FragPos);
	float spec = 0.0f;
	if (blinn)
	{
		vec3 halfWayDir = normalize(lightDir + viewDir);
		spec = pow(max(dot(halfWayDir, fs_in.Normal), 0.0f), 64.0f);
	}
	else
	{
		vec3 reflection = reflect(-lightDir, fs_in.Normal);
		spec = pow(max(dot(viewDir, reflection),0.0f), 16.0f);
	}
	vec3 specular = vec3(0.3f) * spec;
	if (kindOfLight)
	{
		float theta = dot(lightDir, -spot_light.direction);
		float epsilon = spot_light.cutOff - spot_light.outerCutOff;
		float intensity = clamp((theta-spot_light.outerCutOff) / epsilon, 0.0f, 1.0f);
		diffuse *= intensity;
		specular *= intensity;

	}
	float bias = max(0.05f * (1.0f - dot(fs_in.Normal,lightDir)),0.005f);
	float shadow = ShadowCalculate(fs_in.FragPosLightSpace, bias);
	FragColor = vec4(ambient + (1.0f - shadow) * (diffuse + specular), 1.0f);

}