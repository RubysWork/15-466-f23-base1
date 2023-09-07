// #include "PPU466.hpp"
#include "PlayMode.hpp"
#include <glm/glm.hpp>
#include <iostream>

#include <vector>
#include <string>
#include <random>

struct Block
{
	PlayMode playmode;
	int8_t block_offset = 8;

	int index;
	std::string tag = "block";
	glm::vec2 block_position;

	bool stopDetect = false;

	// generate
	Block()
	{
		// the game panel is (0,180),the block length is 45px, width is 4px randomX number between [0,135]
		// random generate position
		int randomX = rand() % (136);
		block_position.x = randomX;
		block_position.y = 244;

		playmode.ppu.sprites[1].x = int8_t(block_position.x);
		playmode.ppu.sprites[1].y = int8_t(block_position.y);
		playmode.ppu.sprites[1].index = 3;		// color
		playmode.ppu.sprites[1].attributes = 3; // tile
		playmode.ppu.sprites[2].x = int8_t(block_position.x + block_offset);
		playmode.ppu.sprites[2].y = int8_t(block_position.y);
		playmode.ppu.sprites[2].index = 3;		// color
		playmode.ppu.sprites[2].attributes = 3; // tile
		playmode.ppu.sprites[3].x = int8_t(block_position.x + block_offset * 2);
		playmode.ppu.sprites[3].y = int8_t(block_position.y);
		playmode.ppu.sprites[3].index = 3;		// color
		playmode.ppu.sprites[3].attributes = 3; // tile
		playmode.ppu.sprites[4].x = int8_t(block_position.x + block_offset * 3);
		playmode.ppu.sprites[4].y = int8_t(block_position.y);
		playmode.ppu.sprites[4].index = 3;		// color
		playmode.ppu.sprites[4].attributes = 3; // tile
	}
	~Block(){};

	/*
		//score, if the player's y is smaller than the block, score++
		void DetectPlayerPosition() {
			if (!stopDetect)
				if (player.y < block.block_position.y) {
					//send something to the score++ function
					//score++;
				}
			block.stopDetect = true;
		}
	*/
};
// generate
/*
	Block()
	{
		// the game panel is (0,180),the block length is 45px, width is 4px randomX number between [0,135]
		// random generate position
		float randomX = (float)(rand() % (91));
		block_position.x = randomX;
		block_position.y = 244;

		ppu.sprites[1].x = int8_t(block_position.x);
		ppu.sprites[1].y = int8_t(block_position.y);
		ppu.sprites[1].index = color_index;		// color
		ppu.sprites[1].attributes = tile_index; // tile
		ppu.sprites[2].x = int8_t(block_position.x + block_offset);
		ppu.sprites[2].y = int8_t(block_position.y);
		ppu.sprites[2].index = color_index;		// color
		ppu.sprites[2].attributes = tile_index; // tile
		ppu.sprites[3].x = int8_t(block_position.x + block_offset * 2);
		ppu.sprites[3].y = int8_t(block_position.y);
		ppu.sprites[3].index = color_index;		// color
		ppu.sprites[3].attributes = tile_index; // tile
		ppu.sprites[4].x = int8_t(block_position.x + block_offset * 3);
		ppu.sprites[4].y = int8_t(block_position.y);
		ppu.sprites[4].index = color_index;		// color
		ppu.sprites[4].attributes = tile_index; // tile
	}
	~Block(){};
	void drawBlock()
	{

		ppu.sprites[1].x = int8_t(block_position.x);
		ppu.sprites[1].y = int8_t(block_position.y);
		ppu.sprites[1].index = color_index;		// color
		ppu.sprites[1].attributes = tile_index; // tile
		ppu.sprites[2].x = int8_t(block_position.x + block_offset);
		ppu.sprites[2].y = int8_t(block_position.y);
		ppu.sprites[2].index = color_index;		// color
		ppu.sprites[2].attributes = tile_index; // tile
		ppu.sprites[3].x = int8_t(block_position.x + block_offset * 2);
		ppu.sprites[3].y = int8_t(block_position.y);
		ppu.sprites[3].index = color_index;		// color
		ppu.sprites[3].attributes = tile_index; // tile
		ppu.sprites[4].x = int8_t(block_position.x + block_offset * 3);
		ppu.sprites[4].y = int8_t(block_position.y);
		ppu.sprites[4].index = color_index;		// color
		ppu.sprites[4].attributes = tile_index; // tile
	}

		//score, if the player's y is smaller than the block, score++
		void DetectPlayerPosition() {
			if (!stopDetect)
				if (player.y < block.block_position.y) {
					//send something to the score++ function
					//score++;
				}
			block.stopDetect = true;
		}

};*/