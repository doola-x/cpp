#include <iostream>
#include <ctime>
#include <chrono>
#include <thread>
#include <chrono>
#include "stack.cpp"

const int WIDTH = 80;
const int HEIGHT = 20;

struct Ball {
	int x, y;
	int dx, dy;
	Ball(int _x, int _y, int _dx, int _dy): x(_x), y(_y), dx(_dx), dy(_dy) {
		std::cout << "ball allocated" << std::endl;
	}
	~Ball() {
		std::cout << "destroyed ball" << std::endl;
	}
};

struct Tuple {
	int x, y, z;
	Tuple(int _x, int _y, int _z): x(_x), y(_y), z(_z) {
		//std::cout << "constructed tuple" << std::endl;
	}
	~Tuple() {
		//std::cout << "destroyed tuple" << std::endl;
	}
};

void draw(const Ball& ball) {
    system("clear");
    for (int i = 0; i < HEIGHT; ++i) {
        for (int j = 0; j < WIDTH; ++j) {
        	if (i == 0 || i == HEIGHT - 1 || j == 0 || j == WIDTH - 1) {
        		if ((j == 0 || j == WIDTH - 1) && (i != 0 && i != HEIGHT -1)) {
        			std::cout << "|";
        		} else {
        			std::cout << "=";
        		}  
            } else if (i == ball.y && j == ball.x) {
                std::cout << "o";
            } else {
                std::cout << " ";
            }
        }
        std::cout << std::endl;
    }
}

void update(Ball& ball) {
    ball.x += ball.dx;
    ball.y += ball.dy;
    if (ball.x <= 1 || ball.x >= WIDTH - 2) ball.dx = -ball.dx;
    if (ball.y <= 1 || ball.y >= HEIGHT - 2) ball.dy = -ball.dy;
}

void dvd_draw(Stack &stack) {
	Ball* ball = stack.allocate<Ball>(WIDTH / 2, HEIGHT / 2, 2, 2);
    while (true) {
        draw(*ball);
        update(*ball);
        std::this_thread::sleep_for(std::chrono::milliseconds(80));
    }
}

void build_tuples(Stack &stack) {
	Tuple* t;
	for (int i = 0; i < 3; i++) {
		t = stack.allocate<Tuple>(i+1, i+2, i+3);
		std::cout << "i: " << i << "\ntuple genereated using stack at: " << t << std::endl; //<< "\nwith values (x,y,z): (" << t->x << "," << t->y << "," << t->z << ")"
		std::cout << "alloc diff: " << reinterpret_cast<uintptr_t>(&stack) - reinterpret_cast<uintptr_t>(&t) << std::endl;	
	}
	int* counter = stack.allocate<int>(0);
	stack.deallocate(counter);
}

void test_dynamic() {
	auto start = std::chrono::high_resolution_clock::now();
	for (int i = 0; i < 100000000; i++) {
		Tuple* tuple = new Tuple(1, 2, 3);
		delete tuple;
	}
	auto end = std::chrono::high_resolution_clock::now();
	std::cout << "dynamic malloc 100m iterations: " << std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count() << "ms" << std::endl;
}

void test_custom(Stack &stack) {
	auto start = std::chrono::high_resolution_clock::now();
	for (int i = 0; i < 100000000; i++) {
		Tuple* tuple = stack.allocate<Tuple>(1, 2, 3);
		stack.deallocate(tuple);
	}
	auto end = std::chrono::high_resolution_clock::now();
	std::cout << "custom malloc 100m iterations: " << std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count() << "ms" << std::endl;
}

int main() {
	Stack stack;
	std::cout << "stack allocated at: " << &stack << std::endl;
	//build_tuples(stack);
	test_dynamic();
	test_custom(stack);
	//dvd_draw(stack);
	return 0;
}