# Music Player
Primeiro projeto da disciplina Infraestrutura de Software - IF677.

### Proposta
Criar um programa concorrente que simule um player de música simples e execute em linha de comando.

### Funcionalidades

- [x] Criar uma playlist.
- [x] Play/Pause a playlist.
- [x] Avançar/Voltar músicas.
- [x] Modo aleatório.
- [x] Remover músicas da playlist.
- [x] Ouvir a playlist

### Pré-requisitos
- g++
> **sudo apt install g++**	

- Ncurses
> **sudo apt install libncurses5-dev**

- SDL
> **sudo apt install libsdl-mixer1.2-dev**

### Como testar
- No terminal, dentro da pasta onde o arquivo **_main.cpp_** está, cole o seguinte comando:
> **g++ main.cpp -lSDLmain -lSDL -lSDL_mixer -lncurses -lpthread**

- Após compilar, execute o arquivo gerado:
> **./a.out**

- Após isso, basta seguir as instruções do programa.

### Restrições/Observações
- O programa só executa músicas .wav
- O programa foi testado apenas no Ubuntu 18.04
- Ainda há correções a serem feitas quanto a execução de uma playlist, no entanto, caso uma música termine e a outra não comece de imediato, basta pressionar "N".