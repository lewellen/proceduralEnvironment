// Adapted from example 27.


// Used as the wave traversal parameter.
uniform float time;

// Deepest part of the seafloor
uniform float minSeaFloor;

// Elevation of the seafloor beneath the vertex
attribute float seaFloor;

// Will only take on values 0 and 1 to indicate that the wave adjustment should be applied.
attribute float applyWave;


varying vec4 clipCoordinates;
varying vec3 norm;
varying vec2 bumpCoordinates;
varying float fresneltheta;
varying float waterDepth;

void main()
{
	// ---------------------------------------------------------------------------
	// Make the surface ripple (function found through experimentation)
	// ---------------------------------------------------------------------------
	vec4 position = gl_Vertex;

	float xAmplitude = 0.005;
	float yAmplitude = 0.001;
	float waveHeight = applyWave * (  yAmplitude * cos(position.y * 10.0 + time) + xAmplitude * sin(position.x * 5.0 + time) );

	position.z = position.z + waveHeight;

	norm = gl_Normal;

	if(applyWave > 0.0 && waveHeight != 0.0) {
		float dx = .05, dy = .05;
		vec3 Dx = vec3(dx, 0.0, dx * xAmplitude * 5.0 * cos(5.0 * position.x + time));	
		vec3 Dy = vec3(0.0, dy, - dy * yAmplitude * 10.0 * sin(10.0 * position.y + time));

		norm = normalize(cross(Dx, Dy));
	}



	// ---------------------------------------------------------------------------
	// Do Phong Lighting
	// ---------------------------------------------------------------------------

	//  P is the vertex coordinate on body
	vec3 P = vec3(gl_ModelViewMatrix * position);

	//  N is the object normal at P
	vec3 N = normalize(gl_NormalMatrix * norm);

	//  Light Position for light 0
	vec3 LightPos = vec3(gl_LightSource[0].position);

	//  L is the light vector
	vec3 L = normalize(LightPos - P);

	//  R is the reflected light vector R = 2(L.N)N - L
	vec3 R = reflect(-L, N);

	//  V is the view vector (eye at the origin)
	vec3 V = normalize(-P);

	//  Diffuse light intensity is cosine of light and normal vectors
	float Id = max(dot(L,N) , 0.0);

	//  Shininess intensity is cosine of light and reflection vectors to a power
	float Is = (Id>0.0) ? pow(max(dot(R,V) , 0.0) , gl_FrontMaterial.shininess) : 0.0;

	//  Vertex color
	vec4 color = gl_FrontLightProduct[0].ambient
		+ Id*gl_FrontLightProduct[0].diffuse
		+ Is*gl_FrontLightProduct[0].specular;

	gl_FrontColor = color;



	// ---------------------------------------------------------------------------
	// Prepare values to pass along to the fragment shader.
	// ---------------------------------------------------------------------------

	// gl_Position is "clip coordinates" want to translate into "window 
	// coordinates" so do perspective division to get normalized device 
	// coordinates then do viewport transformation to get window coordinates.
	// 
	// https://en.wikibooks.org/wiki/GLSL_Programming/Vertex_Transformations
	clipCoordinates = gl_ModelViewProjectionMatrix * position;

	// Need to know where on the quad surface we are so we can pick the right 
	// normal map values.
	bumpCoordinates = position.xy;

	// Will need to know the depth of the water to make the edges transparent.
	waterDepth = (position.z - seaFloor) / (0.0 - minSeaFloor);

	// Need the Fresnel angle to decide how much of above and below portions of 
	// the water to show.
	float PI = 3.14159265358979323846264;	
	fresneltheta = dot(-normalize(P), N);// * 0.5 * PI;



	// ---------------------------------------------------------------------------
	//  Return the clip coordinates for this vertex
	// ---------------------------------------------------------------------------
	gl_Position = clipCoordinates;
}
