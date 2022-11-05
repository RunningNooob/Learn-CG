#version 330 core
out vec4 FragColor;

in vec2 texCoord;
in vec3 normal;
in vec3 fragPos;


struct Material{
	sampler2D diffuse;
	sampler2D specular;
	float shininess;
};
uniform Material material;


//点光源结构体和函数
struct PointLight{
	vec3 position;
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;

	float constant;
	float linear;
	float quadratic;
};
uniform PointLight pointLight;
#define NR_POINT_LIGHTS 4
uniform PointLight pointLights[NR_POINT_LIGHTS];

vec3 calPointLight(PointLight pointLight, vec3 normalDir, vec3 viewDir){
	
	vec3 ambient = pointLight.ambient * vec3(texture(material.diffuse,texCoord));

	vec3 lightDir = normalize(pointLight.position - fragPos);
	vec3 diffuse = max(dot(normalDir,lightDir),0) * pointLight.diffuse * vec3(texture(material.diffuse,texCoord));

	float cos_HN =  dot(normalize(viewDir + lightDir),normalDir);
	vec3 specular = pow(max(cos_HN, 0.0),material.shininess) * pointLight.specular * vec3(texture(material.specular,texCoord));
	
	float distance = length(fragPos - pointLight.position);
	float attenuation = 1.0 / (pointLight.constant + pointLight.linear * distance + pointLight.quadratic * (distance * distance));
	
	ambient *= attenuation;
	diffuse *= attenuation;
	specular *= attenuation;

	return ambient + diffuse + specular;

}


//定向光源结构体和函数
struct DirLight{
	vec3 direction;
	vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};
uniform DirLight dirLight;

vec3 calcDirLight(DirLight dirLight, vec3 normalDir, vec3 viewDir){

	vec3 ambient = dirLight.ambient * vec3(texture(material.diffuse,texCoord));

	vec3 lightDir = normalize(-dirLight.direction);
	vec3 diffuse = max(dot(normalDir,lightDir),0) * dirLight.diffuse * vec3(texture(material.diffuse,texCoord));
	
	float cos_HN =  dot(normalize(viewDir + lightDir),normalDir);
	vec3 specular = pow(max(cos_HN, 0.0),material.shininess) * dirLight.specular * vec3(texture(material.specular,texCoord));

	return ambient + diffuse + specular; 
}



void main()
{
	vec3 result;

	vec3 normalDir = normalize(normal);
	vec3 viewDir = -normalize(fragPos);

	result += calcDirLight(dirLight,normalDir,viewDir);
	for(int i=0; i < NR_POINT_LIGHTS ; ++i){
		result += calPointLight(pointLights[i],normalDir, viewDir);
	}
	

	FragColor = vec4(result, 1.0);
};