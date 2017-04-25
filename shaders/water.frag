// Based on example 27. Set the fragment color

varying vec3 norm;
varying vec4 clipCoordinates;
varying vec2 bumpCoordinates;
varying float fresneltheta;
varying float waterDepth;

uniform float time;

uniform sampler2D reflectionTexture;
uniform sampler2D refractionTexture;
uniform sampler2D bumpTexture;
uniform sampler2D depthTexture;

void main()
{
	// --------------------------------------------------------------------------------
	// Reflection and refraction textures are "screen shots" of the scene. To make use
	// as texutres, need to map clip coordinate to window coordinates so that the 
	// screen shot textures are applied where we'd visually expect them to be.
	// --------------------------------------------------------------------------------

	// https://en.wikibooks.org/wiki/GLSL_Programming/Vertex_Transformations
	vec2 windowCoordinates = 0.5 * (clipCoordinates.xy /  clipCoordinates.w) + 0.5 * vec2(1.0, 1.0);



	// --------------------------------------------------------------------------------
	// Mapping those coordinates 1-1 results is a perfect mirror reflection. Water
	// moves, so a little bit of "swirl" will be applied by using the "bumpTexture".
	// which is really just a normals map.
	// --------------------------------------------------------------------------------

	vec2 adjBumpCoord = vec2(10.0 * bumpCoordinates.x + sin(0.1 * time), 10.0 * bumpCoordinates.y - cos(0.1 * time));
	vec4 displaceColor = texture2D(bumpTexture, adjBumpCoord);
	vec2 displace = 0.01 * displaceColor.rg;



	// --------------------------------------------------------------------------------
	// Calculate the part of the images to combine and get their colors relative to the 
	// window location and the displacement.
	// --------------------------------------------------------------------------------

	vec2 reflecXY = vec2(windowCoordinates.x, -windowCoordinates.y);
	vec2 refracXY = vec2(windowCoordinates.x, windowCoordinates.y);

	vec4 reflecColor = texture2D(reflectionTexture, reflecXY + displace);
	vec4 refracColor = texture2D(refractionTexture, refracXY - displace);
	vec4 depthColor = texture2D(depthTexture, refracXY);

	float xAmplitude = 0.005;
	float yAmplitude = 0.001;
	float waveHeight = 10.0 *  (  yAmplitude * cos(bumpCoordinates.y * 10.0 + time) + xAmplitude * sin(bumpCoordinates.x * 5.0 + time) );


	// --------------------------------------------------------------------------------
	// First put together the refraction (below water) coloring
	// --------------------------------------------------------------------------------

	// Light coming from the surface will project down on the seafloor.
	vec4 causticAmount = texture2D(bumpTexture,  bumpCoordinates + vec2(waveHeight, waveHeight));
	vec4 caustic = 0.5 * (vec4(causticAmount.yyy, 1.0) + vec4(1.0, 1.0, 1.0, 1.0));

	// Use what was gotten from the texture in conjunction with the depth map and the 
	// calculated caustics.
	refracColor = vec4(refracColor.xyz * depthColor.xyz / caustic.xyz, 1.0);

	// Mix in a bit of blue
	refracColor = mix(refracColor, vec4(0.2, 0.4, 0.8, 1.0), 0.25);



	// --------------------------------------------------------------------------------
	// Next put put together the reflection (above water) coloring
	// --------------------------------------------------------------------------------

	// Water on the surface produces specular light. This isn't 100% correct, but gives 
	// the desired effect.
	vec4 specularAmount = texture2D(bumpTexture,  reflecXY - displace);
	vec4 specular = 0.5 * (vec4(specularAmount.yyy, 1.0) + vec4(1.0, 1.0, 1.0, 1.0));

	reflecColor =  gl_Color * specular * reflecColor + 0.1 * specular;



	// --------------------------------------------------------------------------------
	// Finally combine the two
	// --------------------------------------------------------------------------------

	float ip = dot(norm, vec3(0.0, 0.0, 1.0));
	vec4 netColor;
	if(abs(ip) >=  0.99) {
		// only want reflections on the surface
		
		// Mix the two colors together based on how close to the surface the two are
		netColor = mix(reflecColor, refracColor, fresneltheta);


		// Want water closer to the shore line to be transparent to show the 
		// underlying terrain.
		netColor.a = 1.0 / (1.0 + exp(-20.0 * (-.09 + waterDepth)));

	} else {
		// Perimeter is just the refrac color
		netColor = refracColor;
	}

	gl_FragColor = netColor;
}
