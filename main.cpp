#include <iostream>
#include <string>

#include <ncurses.h>
#include <pthread.h>

#include <SDL/SDL.h>
#include <SDL/SDL_mixer.h>

#include <stdlib.h>
#include <sys/types.h>
#include <dirent.h>

using namespace std;

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
	int playlistControl = 0;
// Fim

// Função para usar a PThread
// Basicamente nessa 1ª entrega só foi usada para imprimir a playlist
void *printPlaylist(void *arg){

	char * atual = (char *)arg;

	printw("%s\n", atual);
	pthread_exit(NULL);
}

int main(int argc, char const *argv[]){
	
	char playlist[1000][1000]; // Irá guardar o nome/caminho das músicas. 
						   // Será realmente a nossa playlist

	char nomeMusicas[1000][1000];	
	
	DIR *dir;
	struct dirent *sdirent;
	
	
	initscr();

	int numLinhas, numColunas, y0, x0, y1, x1, y2, x2, y3, x3;

    getmaxyx(stdscr, numLinhas, numColunas);

    y0 = 0, x0 = 0;
    y1 = numLinhas, x1 = numColunas;

    WINDOW *win1 = newwin(y1, x1, y0, x0);
    refresh();

	string tip1 = "Você deve digitar o caminho para adicionar uma música.\n";
	string tip2 = "(Ex: /home/user/Music/)."; 

	mvwprintw(win1, 1, 0, tip1.c_str());
	mvwprintw(win1, 2, 15, tip2.c_str());
	
	wrefresh(win1);
	move(5, 0);

	printw("> ");
	char diretorio[100];
	getstr(diretorio);

	dir = opendir(diretorio);

	if(dir == NULL){
		printw("\nEsse diretório está vazio");
		exit(1);
	}

	// Adicionando músicas do diretório na playlist 
	char aux[100];
	strcpy(aux, diretorio);
	while((sdirent=readdir(dir)) != NULL){
		
		strcpy(diretorio, aux);
		string str = sdirent -> d_name;

		int size = str.length();
		char musicaAtual[size + 1];

		int index = str.find(".wav");

		if((str.find(".wav") != -1)){
			strcat(diretorio, str.c_str());
			strcpy(playlist[music], diretorio);
			str.erase(index, str.length() - 1);

			strcpy(nomeMusicas[music], str.c_str());
			music++;
		}
	}
	closedir(dir);

	move(0, 0);
	y1 = numLinhas, x1 = numColunas;
	win1 = newwin(y1, x1, y0, x0);
	box(win1, '|', '*');
	wrefresh(win1);

	move(2, 2);
	printw("=-=-= Sua Playlist =-=-=\n");
	for (int i = 0; i < music; i++){
		mvprintw(2 + 1 + i, 2, "%d - %s\n", (i+1), nomeMusicas[i]);
	}
	
	refresh(); 
	
	// Basicamente, enquanto for "True", leia chamadas do usuário
	while(exec){
		bool atual = true; // Controla o tempo de execução da música atual
		int next;
		int del;
		int entrada = getch(); // Guarda o valor digitado pelo usuário

		switch(entrada){
			case 'P':
				playlistControl = music;
				music = 0;

				SDL_Init(SDL_INIT_EVERYTHING); // Inicia a biblioteca de áudio
		  	
				Mix_OpenAudio(frequency, format, channel, buffer);

				while(music < playlistControl){ // Enquanto houver música na fila de reprodução
					
					if(next != 'S' && next != 'R'){
						song = Mix_LoadWAV(playlist[music]); // Pega a música atual
						Mix_PlayChannel(-1, song, 0);
					}

					next = getch(); // Usado para caso o usuário queira pula a música atual.

					if(next == 'B'){
						if(music > 0){
							music--; // vai para a próxima música	
							Mix_FreeChunk(song);
						}
					}
					else if(next == 'N'){
						music++; // vai para a próxima música
						Mix_FreeChunk(song);
					}
					else if(next == 'S'){
						if(Mix_Playing(-1)){
							Mix_Pause(-1);
						}
					}
					else if(next == 'R'){
						if(Mix_Paused(-1)){
							Mix_Resume(-1);
						}
					}
				}
		  		printw("\nSua playlist terminou. Se quiser recomeçar essa, pressione P.\nE se quiser sair, pressione E.");
				break;
			case 'D':
				char musicaDelete[100];
				getstr(musicaDelete);

				playlistControl = music;

				for (int i = 0; i < playlistControl; i++){
					if(strcmp(musicaDelete,nomeMusicas[i]) == 0){
						strcpy(playlist[i], "");
						strcpy(nomeMusicas[i], "");
						music--;
					}
				}

				printw("=-=-= Sua Playlist =-=-=\n");
				for (int i = 0; i < music; i++){
					printw("%d - %s\n", (i+1), nomeMusicas[i]);
				}
				break;
		  	case 'E': // Para sair do programa.
		  	printw("Bye bye!\n");
		  	exec = false;
	    	refresh();
			endwin();
		  	break;
		}
	}
	return 0;
}