#include "TerminalGameEngine/tge.hpp"

#include <ctime>
#include <cstdlib>
#include <vector>

int main (){
	static uint8_t size = 4;
		
	static uint8_t width = 18;
	static uint8_t height = 18;

	srand (time (0));

	Game game ("Sssnake Game", width * size, height * size);

	class Object {
		public:
			int16_t x;
			int16_t y;

			Color color;
		
			int8_t dx = 0;
			int8_t dy = 0;
			
			uint16_t length = 0;

			Object (int16_t x, int16_t y, Color color) :
				x (x), y (y), color (color) {}
	};
	

	Object snake ((rand () % width) * size, (rand () % width) * size, {0, 84, 171});
	Object food ((rand () % width) * size, (rand () % width) * size, {196, 0, 0});

	while (food.x == snake.x && food.y == snake.y){
		food.x = (rand () % width) * size;
		food.y = (rand () % height) * size;
	}

	std::vector <Object> tail;


	bool running = 1;
	
	while (running){
		char input = game.getInput ();

		switch (input){
			case 'q':
			case 'Q':
				running = 0;
				break;
				
			case 'w':
			case 'W':
				if (snake.dy == size) break;
				snake.dy = -size;
				snake.dx = 0;
				break;
				
			case 's':
			case 'S':
				if (snake.dy == -size) break;
				snake.dy = size;
				snake.dx = 0;
				break;
				
			case 'a':
			case 'A':
				if (snake.dx == size) break;
				snake.dx = -size;
				snake.dy = 0;
				break;
				
			case 'd':
			case 'D':
				if (snake.dx == -size) break;
				snake.dx = size;
				snake.dy = 0;
				break;	
		}

		snake.x += snake.dx;
		snake.y += snake.dy;
		

		if (snake.x == food.x && snake.y == food.y){
			snake.length++;
		
			uint8_t b = rand () % (256 - 100) + 100;
			int16_t newX = (snake.length > 1) ? tail [snake.length - 1].x : snake.x - snake.dx;
			int16_t newY = (snake.length > 1) ? tail [snake.length - 1].y : snake.y - snake.dy;
					
			tail.push_back (Object (newX, newY, {0, 20, b}));
				
			food.x = (rand () % width) * size;
			food.y = (rand () % height) * size;
																
			for (uint16_t i = 0; i < snake.length; i++){
				if (food.x == tail [i].x && food.y == tail [i].y){
					food.x = (rand () % width) * size;
					food.y = (rand () % height) * size;
					i = 0;
				}
			}
		}

		if (snake.length > 1){
			for (uint16_t i = snake.length; i > 0; i--){
				tail [i].x = tail [i-1].x;
				tail [i].y = tail [i-1].y;
			}
		}
		if (snake.length > 0){
			tail [0].x = snake.x - snake.dx;
			tail [0].y = snake.y - snake.dy;
		}

		if (snake.x < 0) snake.x = width * size - size;
		if (snake.x >= width * size) snake.x = 0;
		if (snake.y < 0) snake.y = height * size - size;
		if (snake.y >= height * size) snake.y = 0;
					

		game.screen.clear ({11, 227, 35});
		
		for (uint16_t i = 0; i < snake.length; i++){
			game.screen.fillColor (tail [i].color);
			game.screen.fillRect (tail [i].x, tail [i].y, size, size);

			if (snake.x == tail [i].x && snake.y == tail [i].y) running = 0;
		}
		
		game.screen.fillColor (food.color);
		game.screen.fillRect (food.x, food.y, size, size);
		
		game.screen.fillColor (snake.color);
		game.screen.fillRect (snake.x, snake.y, size, size);
		
		game.screen.draw ();
		

		game.sleepFor (100);
	}
	
	return 0;
}
