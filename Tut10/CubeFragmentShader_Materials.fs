#version 330 core
struct Material {
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
	float shininess;
};

in vec3 Position;
in vec3 Normal;

out vec4 color;
  
uniform vec3 objectColor;
uniform vec3 lightPos;
uniform vec3 viewPos;

uniform Material material;

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
    vec3 ambientColor = material.ambient;
    vec3 diffuseColor = diffuse * material.diffuse;
    vec3 specularColor = specular * material.specular;
    color = vec4(ambientColor + diffuseColor + specularColor, 1.0f);
}