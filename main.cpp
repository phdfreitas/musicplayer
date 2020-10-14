#include <iostream>
#include <string>
#include <ncurses.h>
#include <SDL/SDL.h>
#include <SDL/SDL_mixer.h>
#include <pthread.h>

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
	
	string playlist[1000]; // Irá guardar o nome/caminho das músicas. 
						   // Será realmente a nossa playlist	
	initscr();

	printw("Você deve digitar o caminho para adicionar uma música.\n(Ex: /home/user/Music/musica.wav).\n");
	printw("Para parar de adicionar músicas a fila de reprodução, digite 0.\n");
	
	// Enquanto o usuário não digitar "0", ele pode continuar adicionando músicas na playlist.
	while(true){
		char musica[100];
		getstr(musica);
		
		if(strcmp(musica, "0") == 0) break;
		
		playlist[music] = musica;
		music++;
	}

	// Simula o menu de opções para o usuário.
	printw("\n--------------------------------------------------------------------\n"); // Representa o "Menu"
		printw("P Play | N Próxima | E Sair | R Remover uma música da playlist."); // Representa o "Menu"
	printw("\n--------------------------------------------------------------------\n");
	
	// Declaro um array de Thread com a quantidade total de músicas + 1 
	pthread_t threads[music + 1];

	printw("\nSua playlist\n");
	
	// Imprime para o usuário a playlist que ele criou
	// Como dito, a PThread foi usuada aqui, mas provavelmente
	// nas pŕóximas versões faremos um uso mais adequado.
	for (int i = 0; i < music; i++){
		int size = playlist[i].length(); // Pega o tamanho da música atual
		char musicaAtual[size + 1]; // Cria um array de char com esse tamanho

		strcpy(musicaAtual, playlist[i].c_str()); // Copia o nome/caminho da música no array

		// Chamamos a pthread_create passando o array de char como parâmetro
		pthread_create(&threads[i], NULL, printPlaylist, musicaAtual);
		pthread_join(threads[i], NULL);
	}

	// Basicamente, enquanto for "True", leia chamadas do usuário
	while(exec){
		bool atual = true; // Controla o tempo de execução da música atual

		int entrada = getch(); // Guarda o valor digitado pelo usuário
		switch(entrada){
			case 'P':
				playlistControl = music;
				music = 0;

				SDL_Init(SDL_INIT_EVERYTHING); // Inicia a biblioteca de áudio
		  	
				Mix_OpenAudio(frequency, format, channel, buffer);

				while(music < playlistControl){ // Enquanto houver música na fila de reprodução
					song = Mix_LoadWAV(playlist[music].c_str()); // Pega a música atual

					Mix_PlayChannel(-1, song, 0); // E configura para reproduzir apenas uma vez

					int next = getch(); // Usado para caso o usuário queira pula a música atual.

					Mix_FreeChunk(song); // "Libera" a música atual
					music++; // vai para a próxima música
				}
		  		printw("\nSua playlist terminou. Se quiser recomeçar essa, pressione P.\nE se quiser sair, pressione E.");
				break;

			case 'R': // Caso o usuário queira remover uma música da playlist
				// Imprimimos a playlist que ele criou

				printw("\nSua playlist\n");
				for (int i = 0; i < music; i++){
					int size = playlist[i].length();
					if(size > 0){
						char musicaAtual[size + 1];

						strcpy(musicaAtual, playlist[i].c_str());

						pthread_create(&threads[i], NULL, printPlaylist, musicaAtual);
						pthread_join(threads[i], NULL);
					}
				}

				printw("\nDigite o nome/caminho da música que você deseja deletar:\n");
				
				char musicaDelete[100];
				getstr(musicaDelete); // Lê qual música ele quer excluir

				// Exclui a música da playlist
				for (int i = 0; i < music; i++){ 
					if(musicaDelete == playlist[i]){
						playlist[i] = "";
					}
				}
				
				// Mostra ao usuário como está a playlist dele depois da exclusão.
				printw("\nSua playlist agora é:\n");

				for (int i = 0; i < music; i++){
					int size = playlist[i].length();
					if(size > 0){
						char musicaAtual[size + 1];

						strcpy(musicaAtual, playlist[i].c_str());

						pthread_create(&threads[i], NULL, printPlaylist, musicaAtual);
						pthread_join(threads[i], NULL);
					}
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