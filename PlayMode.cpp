#include "PlayMode.hpp"
// #include "Block.hpp"
//  for the GL_ERRORS() macro:
#include "gl_errors.hpp"
#include "data_path.hpp"
#include "load_save_png.hpp"

// for glm::value_ptr() :
#include <glm/gtc/type_ptr.hpp>
#include <stdlib.h>
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
	// spike
	create_tile_palette_table(data_path("../data/Spike.png"), data_size, png_data, OriginLocation::LowerLeftOrigin, 4, 4);
	// fragile
	create_tile_palette_table(data_path("../data/Fragile.png"), data_size, png_data, OriginLocation::LowerLeftOrigin, 5, 5);
	// convayor
	create_tile_palette_table(data_path("../data/Convayor.png"), data_size, png_data, OriginLocation::LowerLeftOrigin, 6, 6);

	assign_blocks();
	std::cout << "Welcome to play NS-SHAFT!" << std::endl;
	show_current_hp();
	std::cout << "current Floor:" << score << std::endl;
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
	constexpr float BlockSpeed = 20.0f;
	float DropSpeed = 50.0f;

	// update block position
	for (int i = 0; i < block_positions.size(); i++)
	{
		block_positions[i].x = blocks[i].origin_position.x;
		block_positions[i].y = blocks[i].origin_position.y + block_at.y;

		if (block_positions[i].y > 240)
		{
			block_positions[i].y = (float)((int)block_positions[i].y % 240);
		}
	}

	// game over
	if (player_at.y < 0)
	{
		player_at.y = (float)(240 + ((int8_t)player_at.y % 240));
		game_over();
	}
	// drop off from the top
	else if (player_at.y > 240)
	{
		hp--;

		show_current_hp();
		player_at.y -= 8;
	}

	// if player is right on the block, the player will stop falling
	for (int i = 0; i < blocks.size(); i++)
	{
		if (player_at.x >= block_positions[i].x - 2 && player_at.x <= block_positions[i].x + 34 && player_at.y >= block_positions[i].y && player_at.y <= block_positions[i].y + 8)
		{
			blocks[i].onblock = true;
			on_block_index = i;
		}
		else
		{
			blocks[i].onblock = false;
		}
	}
	// std::cout << "player:(" << player_at.x << "," << player_at.y << ")" << std::endl;
	// std::cout << "block_at:(" << block_positions[6].x << "," << block_positions[6].y << ")" << std::endl;

	// block events
	if (blocks[0].onblock || blocks[1].onblock || blocks[2].onblock || blocks[3].onblock || blocks[4].onblock || blocks[5].onblock || blocks[6].onblock || blocks[7].onblock)
	{ // if player is on the block, the player move with the block
		player_at.y += BlockSpeed * elapsed;

		if (isOnBlockMoment)
		{
			if (blocks[on_block_index].tag == "spike")
			{
				hp--;
				show_current_hp();
			}
			else
			{
				hp++;
				show_current_hp();
			}

			isOnBlockMoment = false;
		}
		if (blocks[on_block_index].tag == "fragile")
		{
			if (fragile_timer < 35 && !fragiled)
				fragile_timer++;
			else
			{
				fragile_timer = 0;
				fragiled = true;
				// record position,respawn in same y
				fragile_index = blocks[on_block_index].index;
				fragiled_position.x = blocks[on_block_index].origin_position.x;
				fragiled_position.y = blocks[on_block_index].origin_position.y;
				// hide the block
				blocks[on_block_index].color_index = 0;
				blocks[on_block_index].tile_index = 0;
				// wait until it disappeared
				player_at.y -= DropSpeed * elapsed * 2;
			}
		}
		else if (blocks[on_block_index].tag == "convayor")
		{
			hp++;
			show_current_hp();
		}
		onblock = true;
	}
	else
	{ // player keep falling
		player_at.y -= DropSpeed * elapsed;
		if (onblock)
		{
			isOnBlockMoment = true;
			onblock = false;
		}
	}

	// when fragile disappeared, remove and reload block
	if (fragiled)
	{
		if (block_positions[fragile_index].y > 238 || block_positions[fragile_index].y < 2)
		{
			// wait until it disappeared
			// remove the block
			array_remove();
			// generate new one
			add_block();
			std::cout << "reload!!!!" << std::endl;
			fragiled = false;
		}
	}

	// block moving
	block_at.y += BlockSpeed * elapsed;

	// set player moving boundary
	if (player_at.x < 1)
	{
		if (right.pressed)
			player_at.x += PlayerSpeed * elapsed;
	}
	else if (player_at.x > 185)
	{
		if (left.pressed)
			player_at.x -= PlayerSpeed * elapsed;
	}
	else
	{
		if (left.pressed)
			player_at.x -= PlayerSpeed * elapsed;
		if (right.pressed)
			player_at.x += PlayerSpeed * elapsed;
	}

	// if (down.pressed) player_at.y -= PlayerSpeed * elapsed;
	// if (up.pressed) player_at.y += PlayerSpeed * elapsed;

	// reset button press counters:
	left.downs = 0;
	right.downs = 0;
	// up.downs = 0;
	// down.downs = 0;

	timer++;
	// std::cout << "timer:" << timer << std::endl;

	if (timer > 1323)
	{
		score++;
		std::cout << "current Floor:" << score << std::endl;
		timer = 0;
	}
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
	// ppu.background_position.y = int32_t(-0.5f * player_at.y);
	ppu.background_position.y = int32_t(-0.5f * player_at.y);

	// generate one block
	for (int i = 0; i < blocks.size(); i++)
	{
		// 4blockbitmaps combine 1 block
		for (int j = 1; j < 5; j++)
		{
			ppu.sprites[j + i * 4].x = int8_t(block_positions[i].x + block_offset * (j - 1));
			ppu.sprites[j + i * 4].y = int8_t(block_positions[i].y);
			ppu.sprites[j + i * 4].index = uint8_t(blocks[i].color_index);	   // color
			ppu.sprites[j + i * 4].attributes = uint8_t(blocks[i].tile_index); // tile
		}
	}
	/*
	//basic way to draw block
		// ppu.sprites[1].x = int8_t(block_at.x);
		// 	ppu.sprites[1].y = int8_t(block_at.y);
		// 	ppu.sprites[1].index = 3;	   // color
		// 	ppu.sprites[1].attributes = 3; // tile
		// 	ppu.sprites[2].x = int8_t(block_at.x + block_offset);
		// 	ppu.sprites[2].y = int8_t(block_at.y);
		// 	ppu.sprites[2].index = 3;	   // color
		// 	ppu.sprites[2].attributes = 3; // tile
		// 	ppu.sprites[3].x = int8_t(block_at.x + block_offset * 2);
		// 	ppu.sprites[3].y = int8_t(block_at.y);
		// 	ppu.sprites[3].index = 3;	   // color
		// 	ppu.sprites[3].attributes = 3; // tile
		// 	ppu.sprites[4].x = int8_t(block_at.x + block_offset * 3);
		// 	ppu.sprites[4].y = int8_t(block_at.y);
		// 	ppu.sprites[4].index = 3;	   // color
		// 	ppu.sprites[4].attributes = 3; // tile
	*/
	// player sprite:
	ppu.sprites[0].x = int8_t(player_at.x);
	ppu.sprites[0].y = int8_t(player_at.y);
	ppu.sprites[0].index = 2;	   // color
	ppu.sprites[0].attributes = 2; // tile

	//--- actually draw ---
	ppu.draw(drawable_size);
}

float PlayMode::ranNum(int min, int max)
{
	float randomX = (float)abs((rand() % (max - min + 1)) + min);
	// std::cout << "min= " << min << " max: " << max << " random: " << randomX << std::endl;
	return randomX;
}

void PlayMode::assign_blocks()
{

	for (int i = 0; i < blocks.size(); i++)
	{
		Block block;
		block.index = i;
		block.origin_position.x = ranNum(0, 160); // every time the random results are same??
		block.origin_position.y = ranNum(i * 30, (i + 1) * 30 - 8);
		block.tag = random_tag();
		if (block.tag == "block")
		{
			block.color_index = 3;
			block.tile_index = 3;
		}
		else if (block.tag == "spike")
		{
			block.color_index = 4;
			block.tile_index = 4;
		}
		else if (block.tag == "fragile")
		{
			block.color_index = 5;
			block.tile_index = 5;
		}
		else if (block.tag == "convayor")
		{
			block.color_index = 6;
			block.tile_index = 6;
		}
		// std::cout << "i= " << i << " y: " << block.origin_position.y << std::endl;
		blocks[i] = block;
	};
	// for (int i = 0; i < (int)ranNum(0, 7); i++)
	// {
	// 	blocks[(int)ranNum(0, 7)].tag = "fragile";
	// 	blocks[(int)ranNum(0, 7)].tag = "convayor";
	// 	blocks[(int)ranNum(0, 7)].tag = "spike";
	// }
}

void PlayMode::game_over()
{
	std::cout << " Game Over! Your final floor is:" << score << std::endl;
	std::cout << " Thank you for playing!" << std::endl;
	system("pause");
}

void PlayMode::show_current_hp()
{
	if (hp > 10)
		hp = 10;
	if (hp <= 0)
		game_over();
	std::string life = "";
	for (int i = 0; i < hp; i++)
	{
		life += "*";
	}

	std::cout << "current HP: " << life << std::endl;
}

void PlayMode::array_remove()
{
	if (on_block_index < blocks.size() - 1)
	{
		for (int i = on_block_index; i < blocks.size() - 1; i++)
		{
			blocks[on_block_index] = blocks[on_block_index + 1];
			block_positions[on_block_index] = block_positions[on_block_index + 1];
		}
	}
}
void PlayMode::add_block()
{
	Block block;
	block.index = fragile_index;
	block.origin_position.x = ranNum(0, 160);
	block.origin_position.y = fragiled_position.y;
	blocks[7].origin_position.x = block.origin_position.x;
	blocks[7].origin_position.y = block.origin_position.y;
	block.tag = random_tag();
	if (block.tag == "block")
	{
		block.color_index = 3;
		block.tile_index = 3;
	}
	else if (block.tag == "spike")
	{
		block.color_index = 4;
		block.tile_index = 4;
	}
	else if (block.tag == "fragile")
	{
		block.color_index = 5;
		block.tile_index = 5;
	}
	else if (block.tag == "convayor")
	{
		block.color_index = 6;
		block.tile_index = 6;
	}
	// std
	// std::cout << "i= " << i << " y: " << block.origin_position.y << std::endl;
	blocks[7] = block;
}

std::string PlayMode::random_tag()
{
	int r = (int)ranNum(0, 7);
	if (r < 3)
		return "block";
	else if (r < 5)
		return "spike";
	else if (r < 7)
		return "fragile";
	else
		return "convayor";
}