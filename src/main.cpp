#include <iostream>
#include <fstream>
#include <time.h>
#include <vector>
#include <string>
#include <SDL.h>
#include <Sistema.hpp>

#define AIR 1.0003

#define PLEXIGLASS_R	1.499
#define PLEXIGLASS_B	1.490
#define PLEXIGLASS		1.494

#define POLYCARBON_R	1.585
#define POLYCARBON_B	1.603

#define CAMPO 28
#define DIMENSIONE_SENSORE  3.4
#define ALTEZZA_SENSORE 150
#define INPUT_DIR std::string("../../data/input/")

using namespace std;
using namespace RelaxedLenses;
// misure in mm

void DisegnaSistema(const Sistema& S, SDL_Renderer* renderer, int off_x, int off_y, float scale);

int main(int argc, char* argv[]) {
	ifstream inf(INPUT_DIR + string("downPoints.dat"));
	if ((inf.is_open()) == false) {
		printf("Error! opening down");
		return 1;
	}
	ifstream sup(INPUT_DIR + string("upPoints.dat"));
	if ((sup.is_open()) == false) {
		printf("Error! opening up");
		return 1;
	}
	SDL_Init(SDL_INIT_VIDEO); // Initialize SDL2
	SDL_GL_SetAttribute(SDL_GL_MULTISAMPLEBUFFERS, 1);
	SDL_GL_SetAttribute(SDL_GL_MULTISAMPLESAMPLES, 16); // 4 samples per pixel (can adjust as needed)
	// Create an SDL window
	SDL_Window* window = SDL_CreateWindow("SDL Line Drawing", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1000, 1000, SDL_WINDOW_SHOWN);

	// Create a renderer for the window
	SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
	Sistema current = Sistema(ALTEZZA_SENSORE, DIMENSIONE_SENSORE, CAMPO);
	current.NumeroRaggi = 8;
	Punti* dw0 = current.NuovoPunti(inf, 0, 35, AIR, AIR, PLEXIGLASS_R, PLEXIGLASS_B);
	Punti* up0 = current.NuovoPunti(sup, 0, 35, PLEXIGLASS_R, PLEXIGLASS_B, AIR, AIR);

	current.OttimizzaPosizioneSensore();
	DisegnaSistema(current, renderer, 500, 300, 4);
	inf.close();
	sup.close();
	inf.open(INPUT_DIR + string("downPoints.dat"));
	sup.open(INPUT_DIR + string("upPoints.dat"));

	Punti* dw1 = current.NuovoPunti(inf, 120.5, 4.0, AIR, AIR, PLEXIGLASS_R, PLEXIGLASS_B);
	Punti* up1 = current.NuovoPunti(sup, 120.5, 4.0, PLEXIGLASS_R, PLEXIGLASS_B, AIR, AIR);
	current.OttimizzaPosizioneLente(dw1, up1);

	current.OttimizzaPosizioneSensore();
	DisegnaSistema(current, renderer, 500, 300, 4);
	//current.OttimizzaPosizioneLente(dw1, up1);
	inf.close();
	sup.close();
	inf.open(INPUT_DIR + string("downPoints.dat"));
	sup.open(INPUT_DIR + string("upPoints.dat"));

	Punti* dw2 = current.NuovoPunti(inf, 132, DIMENSIONE_SENSORE, AIR, AIR, PLEXIGLASS, PLEXIGLASS);
	Punti* up2 = current.NuovoPunti(sup, 132, DIMENSIONE_SENSORE, PLEXIGLASS, PLEXIGLASS, AIR, AIR);
	inf.close();
	sup.close();
	current.Sensore.Quota += 10;
	current.OttimizzaPosizioneSensore();
	current.OttimizzaAmpiezzaSensore();
	current.OttimizzaPosizioneLente(dw2, up2);
	current.OttimizzaPosizioneSensore();
	current.OttimizzaAmpiezzaSensore();
	current.Sensore.Quota += 5;
	current.Elabora();
	
	DisegnaSistema(current, renderer, 500, 300, 4);
	string input;
	cin >> input;
	cout << input;
	// Clean up resources before exiting
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();
	//cout << "sensore at " << current.Sensore.Quota << endl;
	cout << endl << "... COMPLETED!.." << endl;
	return 0;
}

void DisegnaSistema(const Sistema& S, SDL_Renderer* renderer, int off_x, int off_y, float scale) {
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
	SDL_RenderClear(renderer);
	SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
	for (int i = 0; i < S.Data_d.size();i++) {
		for (int j = 1; j < S.Data_d[i].size(); j++) {
			int ax = std::round(scale * S.Data_d[i][j - 1].X + off_x);
			int ay = std::round(scale * S.Data_d[i][j - 1].Y + off_y);
			int bx = std::round(scale * S.Data_d[i][j].X + off_x);
			int by = std::round(scale * S.Data_d[i][j].Y + off_y);
			SDL_RenderDrawLine(renderer, ax, ay, bx, by);
		}
	}
	SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255);
	for (int i = 0; i < S.Data_f.size();i++) {
		for (int j = 1; j < S.Data_f[i].size(); j++) {
			int ax = std::round(-scale * S.Data_f[i][j - 1].X + off_x);
			int ay = std::round(scale * S.Data_f[i][j - 1].Y + off_y);
			int bx = std::round(-scale * S.Data_f[i][j].X + off_x);
			int by = std::round(scale * S.Data_f[i][j].Y + off_y);
			SDL_RenderDrawLine(renderer, ax, ay, bx, by);
		}
	}
	SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
	SDL_RenderDrawLine(renderer, off_x, 0, off_x, 1000);
	SDL_RenderDrawLine(renderer, 0, off_y, 1000, off_y);
	SDL_RenderPresent(renderer);
}

void PlottaScarto(const Sistema& S, SDL_Renderer* renderer, int off_x, int off_y, float scale) {
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
	SDL_RenderClear(renderer);
	SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
	for (int i = 0; i < S.Data_d.size();i++) {
		for (int j = 1; j < S.Data_d[i].size(); j++) {
			int ax = std::round(scale * S.Data_d[i][j - 1].X);
			int ay = std::round(scale * S.Data_d[i][j - 1].Y);
			int bx = std::round(scale * S.Data_d[i][j].X);
			int by = std::round(scale * S.Data_d[i][j].Y);
			SDL_RenderDrawLine(renderer,ax,ay,bx,by);
		}
	}
	SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255);
	for (int i = 0; i < S.Data_f.size();i++) {
		for (int j = 1; j < S.Data_d[i].size(); j++) {
			SDL_RenderDrawLine(renderer, scale * -S.Data_f[i][j - 1].X + off_x, scale * S.Data_f[i][j - 1].Y + off_y, scale * -S.Data_f[i][j].X + off_x, scale * S.Data_f[i][j].Y + off_y);
		}
	}
	/*
	SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
	SDL_RenderDrawLine(renderer, off_x, 0, off_x, 1000);
	SDL_RenderDrawLine(renderer, 0, off_y, 1000, off_y);
	*/
	SDL_RenderPresent(renderer);
}