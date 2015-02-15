#version 330 core
#define NR_POINT_LIGHTS 4

struct Material {
    float shininess;
};

struct Light {
	vec3 position;
	vec3 spotDir;		//Needed for spotlight
    //vec3 direction;	//Needed for directional light

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
	
	//The below 3 are needed for point light
	float constant;
	float linear;
	float quadratic;
	
	//The below is needed for spotlight
	float spotCutOff;
	float spotOuterCutOff;
};

struct DirLight {
	vec3 direction;
	
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
}

struct PointLight {    
    vec3 position;
    
    float constant;
    float linear;
    float quadratic;  

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

in vec3 Position;
in vec3 Normal;
in vec2 TexCoords;

out vec4 color;

uniform sampler2D diffuseSampler;
uniform sampler2D specularSampler;

uniform vec3 viewPos;
uniform Light light;
uniform DirLight dirLight;
uniform PointLight pointLights[NR_POINT_LIGHTS];
uniform Material material;

//Function prototypes
vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir);
vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir);

void main()
{    
    //Properties
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(light.position - Position);		//Not needed for directional light
	//vec3 lightDir = normalize(-light.direction);				//For directional light
    vec3 viewDir = normalize(viewPos - Position);
    vec3 reflectDir = reflect(-lightDir, norm);
	
    float theta = dot(lightDir, normalize(-light.spotDir)); 
	float epsilon = (light.spotCutOff - light.spotOuterCutOff);
	float intensity = clamp((theta - light.spotOuterCutOff) / epsilon, 0.0, 1.0);
    
	//Diffuse shading    
	float diffuse = max(dot(norm, lightDir), 0.0);
	
	//Specular shading    
	float specular = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
	
	//Attenuation
	float distance = length(light.position - Position);
	float attenuation = 1.0f / (light.constant + light.linear * distance + light.quadratic * (distance * distance));	

	//Combine results
	vec3 ambientColor = light.ambient * vec3(texture(diffuseSampler, TexCoords));
	vec3 diffuseColor = light.diffuse * diffuse * vec3(texture(diffuseSampler, TexCoords));
	vec3 specularColor = light.specular * specular * vec3(texture(specularSampler, TexCoords));
	
	ambientColor *= attenuation;
	diffuseColor *= attenuation;
	specularColor *= attenuation;
	
	//Needed for smooth edge spotlight
	diffuseColor *= intensity;
	specularColor *= intensity;
	
	color = vec4(ambientColor + diffuseColor + specularColor, 1.0f);
}

vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir)
{
    vec3 lightDir = normalize(-light.direction);
    
	//Diffuse shading
    float diff = max(dot(normal, lightDir), 0.0);
    
	//Specular shading
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    
	//Combine results
    vec3 ambient = light.ambient * vec3(texture(diffuseSampler, TexCoords));
    vec3 diffuse = light.diffuse * diff * vec3(texture(diffuseSampler, TexCoords));
    vec3 specular = light.specular * spec * vec3(texture(specularSampler, TexCoords));
	
    return (ambient + diffuse + specular);
}

//Calculates the color when using a point light.
vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir)
{
    vec3 lightDir = normalize(light.position - fragPos);
	
    //Diffuse shading
    float diff = max(dot(normal, lightDir), 0.0);
    
	//Specular shading
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    
	//Attenuation
    float distance = length(light.position - fragPos);
    float attenuation = 1.0f / (light.constant + light.linear * distance + light.quadratic * (distance * distance));    
    
	//Combine results
    vec3 ambient = light.ambient * vec3(texture(diffuseSampler, TexCoords));
    vec3 diffuse = light.diffuse * diff * vec3(texture(diffuseSampler, TexCoords));
    vec3 specular = light.specular * spec * vec3(texture(specularSampler, TexCoords));
    ambient *= attenuation;
    diffuse *= attenuation;
    specular *= attenuation;
    
	return (ambient + diffuse + specular);
}