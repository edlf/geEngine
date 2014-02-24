uniform float dT;  
uniform sampler2D heightTexture; 

void main() { 
	vec2 texDiff = vec2(0.0, dT);
	
	// Texture color
	vec4 heightComponent = texture2D(heightTexture, gl_MultiTexCoord0.st + texDiff); 
	
	// Get red component
	vec4 newPoint = vec4(0.0, heightComponent.r, heightComponent.r*0.1, 0.0); 
	
	gl_Position = gl_ProjectionMatrix * gl_ModelViewMatrix * (newPoint + gl_Vertex); 
	
	gl_TexCoord[0].st = gl_MultiTexCoord0.st + texDiff; 
}