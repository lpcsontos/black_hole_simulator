#include <SDL2/SDL.h>
#include <SDL2/SDL2_gfxPrimitives.h>
#include <stdio.h>
#include <cmath>
#include <Eigen/Dense>

struct BH{
	int x, y, r;
	float mass;
};

struct Ray{
	Eigen::Vector2f position;
    Eigen::Vector2f velocity;
	bool go;
};

int main(){

	const int width = 640;
	const int height = 480;

	SDL_Window *window;
	SDL_Init(SDL_INIT_EVERYTHING);
	window = SDL_CreateWindow(
		"other",
		SDL_WINDOWPOS_UNDEFINED,
		SDL_WINDOWPOS_UNDEFINED,
		width,		//width
		height,		//height
		SDL_WINDOW_SHOWN
	);
	SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_SOFTWARE);
	SDL_RenderClear(renderer);
	if(window == NULL){
		printf("shit");
		return 1;
	}
	bool run = true;

	bool rend = 0;
	int bhs_db = 4;
    BH bhs[bhs_db];
	bhs[0].x = 350; bhs[0].y = 250; bhs[0].r = 15; bhs[0].mass = 1000;
	bhs[1].x = 550; bhs[1].y = 350; bhs[1].r = 10; bhs[1].mass = 800;
	bhs[2].x = 150; bhs[2].y = 400; bhs[2].r = 20; bhs[2].mass = 1200;
	bhs[3].x = 150; bhs[3].y = 100; bhs[3].r = 5; bhs[3].mass = 400;


	int rays_db = 40;
	Ray rays[rays_db];
    int ray_ind_help = 0;
	int ddist = static_cast<int>(height / rays_db  + 1);
	for(auto& ray : rays){
		ray.position = Eigen::Vector2f(1, 0 + ddist * ray_ind_help);
		ray.velocity = Eigen::Vector2f(1, 0).normalized();
		ray.go = true;
        ray_ind_help++;
	}

	const int FPS = 90;
	const int frameDelay = 1000 / FPS;
	const float G = 6.67430e-2f;

	while(run){
		Uint32 frameStart = SDL_GetTicks();

		SDL_Event event;
		SDL_PollEvent(&event);
		switch(event.type){
			case SDL_QUIT :
				run = false;
				break;

			default:
				break;
		}

		for(int i = 0; i < bhs_db && rend; i++){
    		filledCircleRGBA(renderer, bhs[i].x, bhs[i].y, bhs[i].r, 255, 0, 0, 255);
		}

		for(int i = 0; i < rays_db; i++){
			if(rays[i].go && static_cast<int>(rays[i].position.x()) < width-20 && static_cast<int>(rays[i].position.x()) > 0){

				Eigen::Vector2f tforce(0,0);
				
				pixelRGBA(renderer, static_cast<int>(rays[i].position.x()), static_cast<int>(rays[i].position.y()), 1, 255, 255, 255);

				for(int j = 0; j < bhs_db; j++){
					Eigen::Vector2f dir = Eigen::Vector2f(bhs[j].x, bhs[j].y) - rays[i].position;
					float distSq = dir.squaredNorm();
					float dist = std::sqrt(distSq);
					dir.normalize();
					tforce += (G * bhs[j].mass / distSq) * dir;
				}
				
				rays[i].velocity += tforce * 1.0f;
				rays[i].position += rays[i].velocity * 1.0f;

				for(int j = 0; j < bhs_db; j++){
					float distance = (rays[i].position - Eigen::Vector2f(bhs[j].x, bhs[j].y)).norm();
					if(distance <= bhs[j].r)rays[i].go = false;
				}
			}
		}
        

		SDL_RenderPresent(renderer);

		Uint32 frameTime = SDL_GetTicks() - frameStart;
    	if (frameDelay > frameTime) {
        	SDL_Delay(frameDelay - frameTime);
    	}
	}

	SDL_DestroyWindow(window);
	SDL_Quit();
	return 0;
}
