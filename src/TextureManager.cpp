#include <cstdio>
#include <stdexcept>

#include "TextureManager.h"

#include "SchreuderUtil.h"

TextureManager* TextureManager::instance = NULL;

TextureManager* TextureManager::Get() {
	if (instance == NULL)
		instance = new TextureManager();

	return instance;
}

TextureManager::TextureManager() {
	lookup = std::map<TextureKey, unsigned int>();
}

unsigned int TextureManager::Load(TextureKey textureKey) {
	// First see if the texture has already been loaded.
	std::map<TextureKey, unsigned int>::iterator i = lookup.find(textureKey);
	if (i != lookup.end())
		return (*i).second;

	// If not, go and load it.
	switch (textureKey) {
	case TEX_BARK:
		lookup[textureKey] = LoadTexBMP("res/plants/bark.bmp");
		break;
	case TEX_BUTTERFLY:
		lookup[textureKey] = LoadTexDecalBMP("res/critters/butterfly.bmp", "res/critters/butterfly-alpha.bmp");
		break;
	case TEX_BUTTERFLY2:
		lookup[textureKey] = LoadTexDecalBMP("res/critters/butterfly2.bmp", "res/critters/butterfly2-alpha.bmp");
		break;
	case TEX_BUTTERFLY3:
		lookup[textureKey] = LoadTexDecalBMP("res/critters/butterfly3.bmp", "res/critters/butterfly3-alpha.bmp");
		break;
	case TEX_CHIMNEY:
		lookup[textureKey] = LoadTexBMP("res/cabin/chimney.bmp");
		break;
	case TEX_DIRT:
		lookup[textureKey] = LoadTexBMP("res/terrain/dirt.bmp");
		break;
	case TEX_DOOR:
		lookup[textureKey] = LoadTexBMP("res/cabin/door.bmp");
		break;
	case TEX_FERN:
		lookup[textureKey] = LoadTexBMP("res/terrain/fern.bmp");
		break;
	case TEX_GRASS:
		lookup[textureKey] = LoadTexBMP("res/terrain/grass.bmp");
		break;
	case TEX_LEAF:
		lookup[textureKey] = LoadTexDecalBMP("res/plants/leaf.bmp", "res/plants/leaf-alpha.bmp");
		break;
	case TEX_LOG:
		lookup[textureKey] = LoadTexBMP("res/cabin/log.bmp");
		break;
	case TEX_LOG_END:
		lookup[textureKey] = LoadTexBMP("res/cabin/log-end.bmp");
		break;
	case TEX_MOSS:
		lookup[textureKey] = LoadTexBMP("res/terrain/moss.bmp");
		break;
	case TEX_PEBBLES:
		lookup[textureKey] = LoadTexBMP("res/terrain/pebbles.bmp");
		break;
	case TEX_ROOF:
		lookup[textureKey] = LoadTexBMP("res/cabin/roof.bmp");
		break;
	case TEX_ROOF_EDGE:
		lookup[textureKey] = LoadTexBMP("res/cabin/roof-side.bmp");
		break;
	case TEX_SAND:
		lookup[textureKey] = LoadTexBMP("res/terrain/sand.bmp");
		break;
	case TEX_SKY:
		lookup[textureKey] = LoadTexBMP("res/sky/sky-box.bmp");
		break;
	case TEX_SLATE:
		lookup[textureKey] = LoadTexBMP("res/terrain/slate.bmp");
		break;
	case TEX_WATER:
		lookup[textureKey] = LoadTexBMP("res/water/water.bmp");
		break;
	case TEX_WATER_BUMP:
		lookup[textureKey] = LoadTexBMP("res/water/water-bump.bmp");
		break;
	case TEX_WEEDS:
		lookup[textureKey] = LoadTexBMP("res/terrain/weeds.bmp");
		break;
	case TEX_WHITE_ROCK:
		lookup[textureKey] = LoadTexBMP("res/terrain/white-rock.bmp");
		break;
	case TEX_WINDOW:
		lookup[textureKey] = LoadTexBMP("res/cabin/window.bmp");
		break;
	default:
		throw new std::invalid_argument("Unknown TextureKey value.");
	}

	// Return the texture handle.
	return lookup[textureKey];
}

void TextureManager::UnloadAll() {
	std::map<TextureKey, unsigned int>::iterator i = lookup.begin();
	std::map<TextureKey, unsigned int>::iterator end = lookup.end();

	for (; i != end; i++) {
		unsigned int textureHandle = (*i).second;
		glDeleteTextures(1, &textureHandle);
	}

	lookup.clear();
}
