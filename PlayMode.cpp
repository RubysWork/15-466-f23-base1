#include "PlayMode.hpp"
// #include "Block.hpp"
//  for the GL_ERRORS() macro:
#include "gl_errors.hpp"
#include "data_path.hpp"
#include "load_save_png.hpp"

// for glm::value_ptr() :
#include <glm/gtc/type_ptr.hpp>
#include <string>
#include <vector>
#include <random>

PlayMode::PlayMode()
{
	// TODO:
	//  you *must* use an asset pipeline of some sort to generate tiles.
	//  don't hardcode them like this!
	//  or, at least, if you do hardcode them like this,
	//   make yourself a script that spits out the code that you paste in here
	//    and check that script into your repository.

	// Also, *don't* use these tiles in your game:
	/*

		{ //use tiles 0-16 as some weird dot pattern thing:
			std::array< uint8_t, 8*8 > distance;
			for (uint32_t y = 0; y < 8; ++y) {
				for (uint32_t x = 0; x < 8; ++x) {
					float d = glm::length(glm::vec2((x + 0.5f) - 4.0f, (y + 0.5f) - 4.0f));
					d /= glm::length(glm::vec2(4.0f, 4.0f));
					distance[x+8*y] = uint8_t(std::max(0,std::min(255,int32_t( 255.0f * d ))));
				}
			}
			for (uint32_t index = 0; index < 16; ++index) {
				PPU466::Tile tile;
				uint8_t t = uint8_t((255 * index) / 16);
				for (uint32_t y = 0; y < 8; ++y) {
					uint8_t bit0 = 0;
					uint8_t bit1 = 0;
					for (uint32_t x = 0; x < 8; ++x) {
						uint8_t d = distance[x+8*y];
						if (d > t) {
							bit0 |= (1 << x);
						} else {
							bit1 |= (1 << x);
						}
					}
					tile.bit0[y] = bit0;
					tile.bit1[y] = bit1;
				}
				ppu.tile_table[index] = tile;
			}
		}

		//use sprite 32 as a "player":
		ppu.tile_table[32].bit0 = {
			0b01111110,
			0b11111111,
			0b11111111,
			0b11111111,
			0b11111111,
			0b11111111,
			0b11111111,
			0b01111110,
		};
		ppu.tile_table[32].bit1 = {
			0b00000000,
			0b00000000,
			0b00011000,
			0b00100100,
			0b00000000,
			0b00100100,
			0b00000000,
			0b00000000,
		};

		//makes the outside of tiles 0-16 solid:
		ppu.palette_table[0] = {
			glm::u8vec4(0x00, 0x00, 0x00, 0x00),
			glm::u8vec4(0x00, 0x00, 0x00, 0xff),
			glm::u8vec4(0x00, 0x00, 0x00, 0x00),
			glm::u8vec4(0x00, 0x00, 0x00, 0xff),
		};

		//makes the center of tiles 0-16 solid:
		ppu.palette_table[1] = {
			glm::u8vec4(0x00, 0x00, 0x00, 0x00),
			glm::u8vec4(0x00, 0x00, 0x00, 0x00),
			glm::u8vec4(0x00, 0x00, 0x00, 0xff),
			glm::u8vec4(0x00, 0x00, 0x00, 0xff),
		};

		//used for the player:
		ppu.palette_table[7] = {
			glm::u8vec4(0x00, 0x00, 0x00, 0x00),
			glm::u8vec4(0xff, 0xff, 0x00, 0xff),
			glm::u8vec4(0x00, 0x00, 0x00, 0xff),
			glm::u8vec4(0x00, 0x00, 0x00, 0xff),
		};

		//used for the misc other sprites:
		ppu.palette_table[6] = {
			glm::u8vec4(0x00, 0x00, 0x00, 0x00),
			glm::u8vec4(0x88, 0x88, 0xff, 0xff),
			glm::u8vec4(0x00, 0x00, 0x00, 0xff),
			glm::u8vec4(0x00, 0x00, 0x00, 0x00),
		};
	*/
	// load png
	glm::uvec2 data_size;			   // size of the image,width and height
	std::vector<glm::u8vec4> png_data; // hold image pixels
	// BG-lightbrown
	create_tile_palette_table(data_path("../data/BG1.png"), data_size, png_data, OriginLocation::LowerLeftOrigin, 0, 0);
	// BG-darkbrown
	create_tile_palette_table(data_path("../data/BG2.png"), data_size, png_data, OriginLocation::LowerLeftOrigin, 1, 1);
	// player
	create_tile_palette_table(data_path("../data/Player.png"), data_size, png_data, OriginLocation::LowerLeftOrigin, 2, 2);
	// block
	create_tile_palette_table(data_path("../data/Block.png"), data_size, png_data, OriginLocation::LowerLeftOrigin, 3, 3);

	// Block *block = new Block();
}

PlayMode::~PlayMode()
{
}

void PlayMode::create_tile_palette_table(std::string filename, glm::uvec2 &data_size, std::vector<glm::u8vec4> &png_data, OriginLocation origin, int palette_index, int tile_index)
{

	// load png

	load_png(filename, &data_size, &png_data, origin);

	// png_data fill color

	std::vector<glm::u8vec4> temp_palette_table;
	temp_palette_table.emplace_back(png_data.at(0));

	// learnt from Jiahan
	for (int i = 0; i < png_data.size(); i++)
	{
		// if (!std::count(PixelColors.begin(), PixelColors.end(), Data.at(i)))
		if (!std::count(temp_palette_table.begin(), temp_palette_table.end(), png_data.at(i)))
			temp_palette_table.emplace_back(png_data.at(i));
	}

	std::copy(temp_palette_table.begin(), temp_palette_table.end(), ppu.palette_table[palette_index].begin());

	// std::cout<<float(ppu.palette_table[1][0].x) <<","<<(float)ppu.palette_table[1][0].y<<","<<(float)ppu.palette_table[1][0].z<<std::endl;
	// std::cout<<float(ppu.palette_table[1][1].x) <<","<<(float)ppu.palette_table[1][1].y<<","<<(float)ppu.palette_table[1][1].z<<std::endl;
	// std::cout<<float(ppu.palette_table[1][2].x) <<","<<(float)ppu.palette_table[1][2].y<<","<<(float)ppu.palette_table[1][2].z<<std::endl;

	// data_size fill shape
	// learnt from Ziqi
	for (int i = 0; i < png_data.size(); i++)
	{
		if (png_data[i] == temp_palette_table[0])
		{
			ppu.tile_table[tile_index].bit0[i / 8] &= ~(1 << i % 8);
			ppu.tile_table[tile_index].bit1[i / 8] &= ~(1 << i % 8);
		}
		else if (png_data[i] == temp_palette_table[1])
		{
			ppu.tile_table[tile_index].bit0[i / 8] |= 1 << i % 8;
			ppu.tile_table[tile_index].bit1[i / 8] &= ~(1 << i % 8);
		}
		else if (png_data[i] == temp_palette_table[2])
		{
			ppu.tile_table[tile_index].bit0[i / 8] &= ~(1 << i % 8);
			ppu.tile_table[tile_index].bit1[i / 8] |= 1 << i % 8;
		}
		else if (png_data[i] == temp_palette_table[3])
		{
			ppu.tile_table[tile_index].bit0[i / 8] |= 1 << i % 8;
			ppu.tile_table[tile_index].bit1[i / 8] |= 1 << i % 8;
		}
	}

	//}
	/**/
}
bool PlayMode::handle_event(SDL_Event const &evt, glm::uvec2 const &window_size)
{

	if (evt.type == SDL_KEYDOWN)
	{
		if (evt.key.keysym.sym == SDLK_LEFT)
		{
			left.downs += 1;
			left.pressed = true;
			return true;
		}
		else if (evt.key.keysym.sym == SDLK_RIGHT)
		{
			right.downs += 1;
			right.pressed = true;
			return true;
		}
		// else if (evt.key.keysym.sym == SDLK_UP) {
		// 	up.downs += 1;
		// 	up.pressed = true;
		// 	return true;
		// } else if (evt.key.keysym.sym == SDLK_DOWN) {
		// 	down.downs += 1;
		// 	down.pressed = true;
		// 	return true;
		// }
	}
	else if (evt.type == SDL_KEYUP)
	{
		if (evt.key.keysym.sym == SDLK_LEFT)
		{
			left.pressed = false;
			return true;
		}
		else if (evt.key.keysym.sym == SDLK_RIGHT)
		{
			right.pressed = false;
			return true;
		}
		//  else if (evt.key.keysym.sym == SDLK_UP) {
		// 	up.pressed = false;
		// 	return true;
		// } else if (evt.key.keysym.sym == SDLK_DOWN) {
		// 	down.pressed = false;
		// 	return true;
		// }
	}

	return false;
}

void PlayMode::update(float elapsed)
{

	// slowly rotates through [0,1):
	//  (will be used to set background color)
	/*
	background_fade += elapsed / 10.0f;
	background_fade -= std::floor(background_fade);
*/
	constexpr float PlayerSpeed = 30.0f;
	constexpr float BlockSpeed = 10.0f;
	float DropSpeed = 50.0f;

	if (player_at.y < 0)
		player_at.y = (float)(240 + ((int8_t)player_at.y % 240));

	if (block_at.y > 240)
		block_at.y = (float)((int8_t)block_at.y % 240);

	std::cout << "player:(" << player_at.x << "," << player_at.y << ")" << std::endl;
	std::cout << "block_at:(" << block_at.x << "," << block_at.y << ")" << std::endl;

	// if player is right on the block, the player will stop falling
	if (player_at.x >= block_at.x - 4 && player_at.x <= block_at.x + 36 && player_at.y >= block_at.y && player_at.y <= block_at.y + 8)
	{
		// if player is on the block, the player move with the block
		player_at.y += BlockSpeed * elapsed;
	}
	else
	{
		// player keep falling
		player_at.y -= DropSpeed * elapsed;
	}

	// block moving
	block_at.y += BlockSpeed * elapsed;

	if (left.pressed)
		player_at.x -= PlayerSpeed * elapsed;
	if (right.pressed)
		player_at.x += PlayerSpeed * elapsed;
	// if (down.pressed) player_at.y -= PlayerSpeed * elapsed;
	// if (up.pressed) player_at.y += PlayerSpeed * elapsed;

	// reset button press counters:
	left.downs = 0;
	right.downs = 0;
	// up.downs = 0;
	// down.downs = 0;
};

void PlayMode::draw(glm::uvec2 const &drawable_size)
{
	//--- set ppu state based on game state ---
	/*
		//background color will be some hsv-like fade:
		ppu.background_color = glm::u8vec4(
			std::min(255,std::max(0,int32_t(255 * 0.5f * (0.5f + std::sin( 2.0f * M_PI * (background_fade + 0.0f / 3.0f) ) ) ))),
			std::min(255,std::max(0,int32_t(255 * 0.5f * (0.5f + std::sin( 2.0f * M_PI * (background_fade + 1.0f / 3.0f) ) ) ))),
			std::min(255,std::max(0,int32_t(255 * 0.5f * (0.5f + std::sin( 2.0f * M_PI * (background_fade + 2.0f / 3.0f) ) ) ))),
			0xff
		);

		//tilemap gets recomputed every frame as some weird plasma thing:
		//NOTE: don't do this in your game! actually make a map or something :-)
		for (uint32_t y = 0; y < PPU466::BackgroundHeight; ++y) {
			for (uint32_t x = 0; x < PPU466::BackgroundWidth; ++x) {
				//TODO: make weird plasma thing
				ppu.background[x+PPU466::BackgroundWidth*y] = ((x+y)%16);
			}
		}

		//background scroll:
		ppu.background_position.x = int32_t(-0.5f * player_at.x);
		ppu.background_position.y = int32_t(-0.5f * player_at.y);



		//some other misc sprites:
		for (uint32_t i = 1; i < 63; ++i) {
			float amt = (i + 2.0f * background_fade) / 62.0f;
			ppu.sprites[i].x = int8_t(0.5f * PPU466::ScreenWidth + std::cos( 2.0f * M_PI * amt * 5.0f + 0.01f * player_at.x) * 0.4f * PPU466::ScreenWidth);
			ppu.sprites[i].y = int8_t(0.5f * PPU466::ScreenHeight + std::sin( 2.0f * M_PI * amt * 3.0f + 0.01f * player_at.y) * 0.4f * PPU466::ScreenWidth);
			ppu.sprites[i].index = 32;
			ppu.sprites[i].attributes = 6;
			if (i % 2) ppu.sprites[i].attributes |= 0x80; //'behind' bit
		}
	*/

	// uint8_t block_originalX = 90;
	// uint8_t block_originalY = 120;

	// BG-lightBrown x[0,180]
	for (uint32_t y = 0; y < PPU466::BackgroundHeight; ++y)
	{
		for (uint32_t x = 0; x < PPU466::BackgroundWidth - (PPU466::BackgroundWidth / 8); ++x)
		{
			// TODO: make weird plasma thing
			ppu.background[x + PPU466::BackgroundWidth * y] = 0b0000000000000000;
		}
	}
	// BG-darkBrown x[180,240]
	for (uint32_t y = 0; y < PPU466::BackgroundHeight; ++y)
	{
		for (uint32_t x = (PPU466::BackgroundWidth / 8) * 3; x < PPU466::BackgroundWidth; ++x)
		{
			// TODO: make weird plasma thing
			ppu.background[x + PPU466::BackgroundWidth * y] = 0b0000000100000001;
		}
	}

	ppu.background_position.x = 0;
	ppu.background_position.y = int32_t(-0.5f * player_at.y);

	// block sprite:
	// random generated
	/*
	Block *block = new Block();
	block->block_position.x = int8_t(block_at.x);
	block->block_position.y = int8_t(block_at.y);
	block->color_index = 3;
	block->tile_index = 3;
	block->drawBlock();
	*/
	int8_t block_offset = 8;

	std::array<std::array<PPU466::Sprite, 4>, 8> blocks;
	for (int i = 0; i < 8; i++)
	{
		Block block;
		for (int j = 0; j < 4; j++)
		{
			block.index = i;
			block.position = (glm::vec2)(ranNum(0, 135), ranNum(0, 240));
			block.sprites[j] = ppu.sprites[j + 1 + i * 4];
		}

		blocks[i] = block.sprites;
	}

	// std::cout << "blocks" << blocks[0][0].x << "," << blocks[1][0].x << std::endl;
	//  for()

	ppu.sprites[1].x = int8_t(block_at.x);
	ppu.sprites[1].y = int8_t(block_at.y);
	ppu.sprites[1].index = 3;	   // color
	ppu.sprites[1].attributes = 3; // tile
	ppu.sprites[2].x = int8_t(block_at.x + block_offset);
	ppu.sprites[2].y = int8_t(block_at.y);
	ppu.sprites[2].index = 3;	   // color
	ppu.sprites[2].attributes = 3; // tile
	ppu.sprites[3].x = int8_t(block_at.x + block_offset * 2);
	ppu.sprites[3].y = int8_t(block_at.y);
	ppu.sprites[3].index = 3;	   // color
	ppu.sprites[3].attributes = 3; // tile
	ppu.sprites[4].x = int8_t(block_at.x + block_offset * 3);
	ppu.sprites[4].y = int8_t(block_at.y);
	ppu.sprites[4].index = 3;	   // color
	ppu.sprites[4].attributes = 3; // tile

	// player sprite:
	ppu.sprites[0].x = int8_t(player_at.x);
	ppu.sprites[0].y = int8_t(player_at.y);
	ppu.sprites[0].index = 2;	   // color
	ppu.sprites[0].attributes = 2; // tile

	//--- actually draw ---
	ppu.draw(drawable_size);
}

int8_t PlayMode::ranNum(int min, int max)
{
	int8_t randomX = int8_t(rand() % (max - min + 1));
	return randomX;
}