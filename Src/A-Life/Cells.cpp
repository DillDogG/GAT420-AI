#include "Cells.h"
#include "Texture.h"

bool Cells::Initialize()
{
	buffer.resize(size.x * size.y);

	return true;
}

void Cells::Step()
{
	// "draw" onto buffer
	Write<uint8_t>(buffer, (size.x / 2), 0, 1);
	// rule 30 = 00011110 (binary)
	uint8_t rule = 60;
	//uint8_t rule[] = { 0, 1, 1, 1, 1, 0, 0, 0 };
	// update buffer
	for (int y = 0; y < size.y - 1; y++)
	{
		for (int x = 0; x < size.x; x++)
		{
			uint8_t i = 0;
			// read surrounding cells (x-1, x, x+2), a value between 0-7 can be created by shifting the bits
			i |= Read<uint8_t>(buffer, x - 1, y) << 2;
			i |= Read<uint8_t>(buffer, x, y) << 1;
			i |= Read<uint8_t>(buffer, x + 1, y) << 0;

			// elementary cellular automata rules
			uint8_t state = (rule & 1 << i) ? 1 : 0;
			//uint8_t state = (rule[7 - i]) ? 1 : 0;
			Write<uint8_t>(buffer, x, (y + 1), state);
		}
	}
	//if (mouse_button[1])
	//{
	//	Write<uint8_t>(buffer, mouse_position.x, mouse_position.y, 255);
	//}

	// convert buffer data format to color buffer
	std::transform(buffer.begin(), buffer.end(), color_buffer.begin(), [](uint8_t v)
		{
			return (v) ? 0xffffffff : 0;
		});

	// copy color buffer to texture
	texture->Copy(color_buffer);
}

