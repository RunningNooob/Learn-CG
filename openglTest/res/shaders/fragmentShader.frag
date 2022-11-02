#version 330 core
out vec4 FragColor;

in vec2 texCoord;
in vec3 normal;
in vec3 fragPos;

uniform sampler2D texture1;
uniform sampler2D texture2;	

uniform vec3 objectColor;
uniform vec3 lightColor;
uniform vec3 lightPos;

void main()
{
//	FragColor = mix(texture(texture1, texCoord),texture(texture2,texCoord),0.2);

	float ambientStrength = 0.1;
	vec3 ambient = ambientStrength * lightColor;

	vec3 normalDir = normalize(normal);
	vec3 lightDir = normalize(lightPos - fragPos);
	vec3 diffuse = max(dot(normalDir,lightDir),0) * lightColor;

	float specularStrength = 0.5;
	vec3 viewDir = -normalize(fragPos);

//	vec3 halfwayDir = normalize(lightDir + viewDir);
//	float spec = pow(max(dot(normalDir, halfwayDir), 0.0), 32);
//	vec3 specular = lightColor * spec * specularStrength;

	float cos_HN =  dot(normalize(viewDir + lightDir),normalDir);
	vec3 specular = pow(max(cos_HN,0),32) * specularStrength * lightColor;

	
//	vec3 reflectDir = reflect(-lightDir, normalDir);
//	float cos_VR = dot(viewDir, reflectDir);
//	vec3 specular = pow(max(cos_VR,0),32) * specularStrength * lightColor;
	
	vec3 result = (ambient + diffuse + specular) * objectColor;

	FragColor = vec4(result, 1.0);
};