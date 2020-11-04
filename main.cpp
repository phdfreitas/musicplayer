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


// Função que permite excluir uma música da playlist após a mesma já ter iniciado
void *deleteSong(void *arg){
	char * toDelete = (char *)arg;
	strcpy(toDelete, "");
	pthread_exit(NULL);
}

int main(int argc, char const *argv[]){
	
	char playlist[1000][1000]; // Guarda o caminho completo da música
	char nomeMusicas[1000][1000]; // Guarda o nome da música (Fins de retorno ao usuário)

	pthread_t threadDelete[2]; // Usaremos para pode excluir a música enquanto a playlist rola	
	
	// Nos permite pegar todos os arquivos de um diretório
	DIR *dir;
	struct dirent *sdirent;
	
	//iniciar a tela
	initscr();
	//tamanho das telas
	int height,width,start_y,start_x;
	height=30;
	width=100;
	start_y = 10;
	start_x = 20;
	//iniciando a tela incial
	WINDOW * win = newwin(height, width, start_y,start_x);
	//refresh na tela
	refresh();
	//adicionando uma borda
	box(win,0,0);
	mvwprintw(win,0,45,"Play Music");
	//refresh na tela win1
	wrefresh(win);
	//printar na tela
	mvwprintw(win,1,20,"Bem-vindo ao Play Music!");
	mvwprintw(win,2,1,"--------------------------------------------------------------------------------------------------");
	mvwprintw(win,5,20,"Vamos criar a sua Fila de Reprodução... Adicione as suas músicas a seguir!");
	mvwprintw(win,7,20,"Você deve digitar o caminho para adicionar uma música.");
	mvwprintw(win,8,20,"(Ex: /home/user/Music/).");
	mvwprintw(win,9,20,"");
	wrefresh(win);
	//deslocamento do cursor
	move(20,40);

	// Entrada de diretório do usuário
	char diretorio[100]; 
	getstr(diretorio);

	dir = opendir(diretorio);

	if(dir == NULL){
		move(21,40);
		printw("\nEsse diretório está vazio");
		move(22,40);
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

	//iniciando a tela 2
	WINDOW * win2 = newwin(height, width, start_y,start_x);
	box(win2,0,0);
	mvwprintw(win2,0,45,"Play Music");
	//refresh na tela 2
	wrefresh(win2);

	//construindo o menu rodape do topo
	mvwprintw(win2,1,1,"--------------------------------------------------------------------------------------------------");
	mvwprintw(win2,2,20,"P Play | S Pause | R Resume | | N Next | B Back | | D Delete | E Exit "); // Representa o "Menu"
	mvwprintw(win2,3,1,"--------------------------------------------------------------------------------------------------");
	wrefresh(win2);
	mvwprintw(win2,5,2,"=-=-= Sua Playlist =-=-=");
	wrefresh(win2);

	// Lista para o usuário quais as músicas na sua playlist
	for (int i = 0; i < music; i++){
		mvprintw(16 + 1 + i, 22, "%d - %s\n", (i+1), nomeMusicas[i]);
	}
	//deslocar o cursor
	move(16,22);
	//refresh na tela2
	wrefresh(win2); 
	
	// Basicamente, enquanto for "True", leia chamadas do usuário
	while(exec){
		bool atual = true; // Controla o tempo de execução da música atual
		int next;
		int entrada = getch(); // Guarda o valor digitado pelo usuário
		
		switch(entrada){
			case 'P': // Inicia a playlist de execução
				playlistControl = music;
				music = 0;

				SDL_Init(SDL_INIT_EVERYTHING); // Inicia a biblioteca de áudio
		  	
				Mix_OpenAudio(frequency, format, channel, buffer);

				while(music < playlistControl){ // Enquanto houver música na fila de reprodução
					
					// Não deixa músicas tocarem paralelamente após o Pause/Resume/Delete
					if(next != 'S' && next != 'R' || next == 'Y'){
						song = Mix_LoadWAV(playlist[music]); // Pega a música atual
						Mix_PlayChannel(-1, song, 0);
					}

					next = getch(); // Espera uma entrada do usuário.

					if(next == 'B'){
						if(music > 0){
							music--; // Volta uma música	
							Mix_FreeChunk(song);
						}
					}
					else if(next == 'N'){
						music++; // Avança uma música
						Mix_FreeChunk(song);
					}
					else if(next == 'S'){ // Pause
						if(Mix_Playing(-1)){
							Mix_Pause(-1);
						}
					}
					else if(next == 'R'){ // Resume
						if(Mix_Paused(-1)){
							Mix_Resume(-1);
						}
					}
					else if(next == 'D'){ // Delete - Permite deletar uma música com a playlist em execução
						char musicaDelete[100];
						getstr(musicaDelete);
						move(16,22);
						for (int i = 0; i < playlistControl; i++){
							if(strcmp(musicaDelete,nomeMusicas[i]) == 0){
								
								Mix_FreeChunk(song);
								if(i == music) music++;
								
								pthread_create(&threadDelete[0], NULL, deleteSong, playlist[i]);
								pthread_create(&threadDelete[1], NULL, deleteSong, nomeMusicas[i]);
								pthread_join(*threadDelete, NULL);
							}
						}
					}
				}
				//move o cursor
				move(30,22);
		  		printw("Sua playlist terminou. Se quiser recomeçar essa, pressione P. E se quiser sair, pressione E.");
				break;
			case 'D': // Delete - Antes da playlist começar
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