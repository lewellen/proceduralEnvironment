#ifndef __TextureManager_H__
#define __TextureManager_H__

#include <map>

// List of textures used by the application
enum TextureKey {
	TEX_BARK,
	TEX_BUTTERFLY,
	TEX_BUTTERFLY2,
	TEX_BUTTERFLY3,
	TEX_CHIMNEY,
	TEX_DIRT,
	TEX_DOOR,
	TEX_FERN,
	TEX_GRASS,
	TEX_LEAF,
	TEX_LOG,
	TEX_LOG_END,
	TEX_MOSS,
	TEX_PEBBLES,
	TEX_ROOF,
	TEX_ROOF_EDGE,
	TEX_SAND,
	TEX_SKY,
	TEX_SLATE,
	TEX_WATER,
	TEX_WATER_BUMP,
	TEX_WEEDS,
	TEX_WHITE_ROCK,
	TEX_WINDOW,
};

// Singleton class that maps a texture name to an OpenGL texture handle.
class TextureManager {
private:
	// Singleton instance
	static TextureManager* instance;

	// Map between names and handles.
	std::map<TextureKey, unsigned int> lookup;

	// Private constructor
	TextureManager();

public:
	// Get the singleton.
	static TextureManager* Get();

	// Get the mapping between name and handle. If first call, then load the 
	// resource and bind, otherwise just return handle.
	unsigned int Load(TextureKey textureKey);

	// Releases all the texture handles.
	void UnloadAll();
};

#endif
