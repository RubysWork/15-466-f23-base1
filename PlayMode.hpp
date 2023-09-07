#include "PPU466.hpp"
#include "Mode.hpp"
#include "load_save_png.hpp"
#include <glm/glm.hpp>
#include <string>
#include <vector>
#include <deque>

struct PlayMode : Mode
{
	PlayMode();
	virtual ~PlayMode();

	// functions called by main loop:
	virtual void create_tile_palette_table(std::string filename, glm::uvec2 &size, std::vector<glm::u8vec4> &data, OriginLocation origin, int palette_index, int tile_index) override;

	virtual bool handle_event(SDL_Event const &, glm::uvec2 const &window_size) override;
	virtual void update(float elapsed) override;
	virtual void draw(glm::uvec2 const &drawable_size) override;
	virtual float ranNum(int min, int max);

	virtual void assign_blocks();

	//----- game state -----

	// input tracking:
	struct Button
	{
		uint8_t downs = 0;
		uint8_t pressed = 0;
	} left, right, down, up;

	// some weird background animation:
	// float background_fade = 0.0f;
	int8_t block_offset = 8;
	// block position:
	glm::vec2 block_at = glm::vec2(0, 0);

	// player position:
	glm::vec2 player_at = glm::vec2(90, 180);

	//----- drawing handled by PPU466 -----

	PPU466 ppu;

	//----- object ----
	typedef struct Block
	{
		int index;
		glm::vec2 origin_position;
		bool onblock;
		// std::array<PPU466::Sprite, 4> sprites;

	} Block;

	// blocks on the screen
	std::array<Block, 8> blocks;
	std::array<glm::vec2, 8> block_positions;
};
