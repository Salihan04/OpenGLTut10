#version 330 core
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
};

in vec3 Position;
in vec3 Normal;
in vec2 TexCoords;

out vec4 color;

uniform sampler2D diffuseSampler;
uniform sampler2D specularSampler;

uniform vec3 viewPos;
uniform Light light;
uniform Material material;

void main()
{    
    //Properties
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(light.position - Position);		//Not needed for directional light
	//vec3 lightDir = normalize(-light.direction);				//For directional light
    vec3 viewDir = normalize(viewPos - Position);
    vec3 reflectDir = reflect(-lightDir, norm);
	
	//Check if lighting is inside the spotlight cone
    float theta = dot(lightDir, normalize(-light.spotDir)); 
    
	//Remember that we're working with angles as cosines instead of degrees so a '>' is used.
	if(theta > light.spotCutOff)
    { 
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
		
		color = vec4(ambientColor + diffuseColor + specularColor, 1.0f);
	}
	//else, use ambient light so scene isn't completely dark outside the spotlight.
	else
        color = vec4(light.ambient * vec3(texture(diffuseSampler, TexCoords)), 1.0f);
}