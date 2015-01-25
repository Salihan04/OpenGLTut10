#version 330 core
in vec3 Position;
in vec3 Normal;

out vec4 color;
  
uniform vec3 objectColor;
uniform vec3 lightPos;

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
	
	vec3 result = (ambientComponent + diffuseComponent) * objectColor;
	
    color = vec4(result, 1.0f);
}