#version 330 core
in vec3 Position;
in vec3 Normal;

out vec4 color;
  
uniform vec3 objectColor;
uniform vec3 lightPos;
uniform vec3 viewPos;

void main()
{
	//Adding ambient lighting
	vec3 ambientComponent = vec3(0.2f);
	
	//Adding diffuse lighting
	vec3 diffuseComponent = vec3(0.5f);
	
	vec3 norm = normalize(Normal);
	vec3 lightDir = normalize(lightPos - Position);
	
	float diffuse = max(dot(norm, lightDir), 0.0);
	diffuseComponent = diffuse * diffuseComponent;
	
	//Adding specular lighting
	vec3 specularComponent = vec3(0.5f);
	
	vec3 viewDir = normalize(viewPos - Position);
	vec3 reflectDir = reflect(-lightDir, norm);
	
	int shininess = 32;		//the higher the value, the smaller the shiny spot
	float specular = pow(max(dot(viewDir, reflectDir), 0.0), shininess);
	specularComponent = specular * specularComponent;
	
	vec3 result = (ambientComponent + diffuseComponent + specularComponent) * objectColor;
	
    color = vec4(result, 1.0f);
}