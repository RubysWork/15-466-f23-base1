
#include <glm/glm.hpp>
#include <iostream>

#include <vector>
#include <string>
#include <random>

struct Block {

	int index;
	glm::vec2 block_position;
	//source="Player.png";

	std::string tag="block";

	static glm::vec2 block_boundingBox;//something wrong with this line (18, 4)
	
	bool stopDetect = false;

	Block(){
		//the game panel is (0,180),the block length is 45px, width is 4px randomX number between [45,135]
		int randomX = rand() % (91);
		block_position.x = 45 + randomX;
		block_position.y = 244;
	}
	~Block() {};

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