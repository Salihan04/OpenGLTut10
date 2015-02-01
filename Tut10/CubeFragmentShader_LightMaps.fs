#version 330 core
struct Material {
	vec3 specular;
	float shininess;
};

struct Light {
	vec3 position;
	
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
};

in vec3 Position;
in vec3 Normal;
in vec2 TexCoords;

out vec4 color;

uniform sampler2D d;
uniform vec3 objectColor;
uniform vec3 lightPos;
uniform vec3 viewPos;

uniform Material material;
uniform Light light;

void main()
{
	//Properties
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(lightPos - Position);
    vec3 viewDir = normalize(viewPos - Position);
    vec3 reflectDir = reflect(-lightDir, norm);
    
    //Diffuse shading    
    float diffuse = max(dot(norm, lightDir), 0.0);
    
    //Specular shading    
    float specular = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);

    //Combine results
    vec3 ambientColor = light.ambient * vec3(texture(d, TexCoords));
	vec3 diffuseColor = light.diffuse * diffuse * vec3(texture(d, TexCoords));
	vec3 specularColor = light.specular * specular * material.specular;
    color = vec4(ambientColor + diffuseColor + specularColor, 1.0f);
}