#include <iostream>
#include <string>
#include <ncurses.h>
#include <SDL/SDL.h>
#include <SDL/SDL_mixer.h>

using namespace std;

int main(int argc, char const *argv[]){
	
	// Configuração base para tocar as músicas
		int frequency = 44100; 
		Uint16 format = AUDIO_S16SYS;
		int channel = 2; 
		int buffer = 4096;
		Mix_Chunk *song;
	// Fim configuração base

	// Variáveis para controle da execução do programa 
		bool exec = true;
		int music = 0;
	// Fim


	initscr();
	printw("P Play | N Next | E Exit"); // Representa o "Menu"

	string dir[4] = {"music/bensound-onceagain.wav", "music/bensound-epic.wav", 
	"music/bensound-onceagain.wav", "music/bensound-epic.wav"};

	printw("\nYour playlist\n");
	for (int i = 0; i < 4; i++){
		printw("%d - %s\n", (i + 1), dir[i].c_str());
	}


	while(exec){
		bool atual = true; // Controla o tempo de execução da música atual

		int entrada = getch(); // Guarda o valor digita pelo usuário
		switch(entrada){
			case 'P':
				music = 0;
				SDL_Init(SDL_INIT_EVERYTHING); // Inicia a biblioteca p/ o áudio
		  	
				Mix_OpenAudio(frequency, format, channel, buffer);

				while(music < 4){ // Enquanto houver música na fila de reprodução
					song = Mix_LoadWAV(dir[music].c_str()); // Pega a música atual

					Mix_PlayChannel(-1, song, 0); // E configura para reproduzir apenas uma vez

					int next = getch();
					Mix_FreeChunk(song); // "Libera" a música atual
					music++; // vai para a próxima música
				}
		  		printw("\nSua playlist terminou. Se quiser recomeçar essa, pressione P.\nE se quiser sair, pressione E.");
				break;
		  	case 'E':
		  	printw("Bye bye!\n");
		  	exec = false;
	    	refresh();
			endwin();
		  	break;
		}
	}
	return 0;
}