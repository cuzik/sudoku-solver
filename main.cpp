#include <iostream>
#include "board.hpp"



#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>


bool run = true;

int TAM_CELULA = 70;
int TAM_BORDA = 30;
int x_Dimension = 9;
int y_Dimension = 9;

int LARGURA_TELA = x_Dimension * TAM_CELULA + 2 * TAM_BORDA;
int ALTURA_TELA = y_Dimension * TAM_CELULA + 2 * TAM_BORDA;

ALLEGRO_DISPLAY *janela = NULL;
ALLEGRO_EVENT_QUEUE *fila_eventos = NULL;
ALLEGRO_FONT *fonte = NULL;
ALLEGRO_FONT *fonte2 = NULL;

void read_keyboard();
bool init_allegro();
void finish_allegro();


int number_errors = 0;
int ramdon_moves = 0;

void ramdon_search(Board board);
Board ramdon_search_aux(Board board);








int main(int argc, const char *argv[]){
  if(argc < 2){
    std::cout << "falta o nome do arquivo" << std::endl;
    exit(1);
  }
  init_allegro();
  srand( (unsigned)time(NULL) );
  Board board;
  board.read_board(argv[1]);
  board.init_potential();
  board.put_the_single_potential();

  ramdon_search(board);

  finish_allegro();
  return 0;
}

void ramdon_search(Board board){
  while(run){
    Board resp = ramdon_search_aux(board);
    if(resp.valid){
      board = resp;
      while(run){
        board.show_board();
      }
    }
    board.show_board();
  }
}

Board ramdon_search_aux(Board board){
  number_errors++;
  ramdon_moves = 0;
  for(int i=0; i<9*9; i++){
    ramdon_moves++;
    board.put_ramdon_element();
    if(board.move_wrong()){
      break;
    }
    board.show_board();
    board.put_the_single_potential();
    if(board.end_game()){
      return board;
    }
    if(board.move_wrong()){
      break;
    }
    board.show_board();
  }
  Board res_f;
  res_f.valid = false;
  return res_f;
}

/*ALLEGRO FUNCTIONS*/
void read_keyboard(){
    while(!al_is_event_queue_empty(fila_eventos)){
        ALLEGRO_EVENT evento;
        al_wait_for_event(fila_eventos, &evento);
        if(evento.type == ALLEGRO_EVENT_KEY_DOWN){
            if(evento.keyboard.keycode == ALLEGRO_KEY_ESCAPE){
                run = false;
            }
        }else if(evento.type == ALLEGRO_EVENT_DISPLAY_CLOSE){
            run = false;
        }
    }
}

bool init_allegro(){
    if (!al_init()){
        fprintf(stderr, "Falha ao inicializar Allegro.\n");
        return false;
    }

    al_init_font_addon();

    if (!al_init_ttf_addon()){
        fprintf(stderr, "Falha ao inicializar allegro_ttf.\n");
        return false;
    }

    if (!al_init_primitives_addon()){
        fprintf(stderr, "Falha ao inicializar add-on allegro_primitives.\n");
        return false;
    }

    if (!al_install_keyboard()){
        fprintf(stderr, "Falha ao inicializar o teclado.\n");
        return false;
    }

    janela = al_create_display(LARGURA_TELA, ALTURA_TELA);
    if (!janela){
        fprintf(stderr, "Falha ao criar janela.\n");
        return false;
    }

    al_set_window_title(janela, "Sudoku - AI Solver");

    fila_eventos = al_create_event_queue();
    if (!fila_eventos){
        fprintf(stderr, "Falha ao criar fila de eventos.\n");
        al_destroy_display(janela);
        return false;
    }

    fonte = al_load_font("Font.ttf", TAM_CELULA, 0);
    if (!fonte)
    {
        fprintf(stderr, "Falha ao carregar fonte.\n");
        al_destroy_event_queue(fila_eventos);
        al_destroy_display(janela);
        return false;
    }

    fonte2 = al_load_font("Font.ttf", TAM_CELULA*.5, 0);
    if (!fonte2)
    {
        fprintf(stderr, "Falha ao carregar fonte2.\n");
        al_destroy_event_queue(fila_eventos);
        al_destroy_display(janela);
        return false;
    }

    al_register_event_source(fila_eventos, al_get_keyboard_event_source());
    al_register_event_source(fila_eventos, al_get_display_event_source(janela));

    return true;
}

void finish_allegro(){
  al_destroy_event_queue(fila_eventos);
  al_destroy_display(janela);
}


/*BOARD FUNCTIONS THAT USE A GLOBAL VARIABLES*/
void Board::show_board(){
  read_keyboard();
  al_clear_to_color(al_map_rgb(200, 200, 200));

  al_draw_textf(fonte2, al_map_rgb(0, 0, 0), TAM_BORDA , 0, 0, "try: %i | fitness: %i | RM: %i", number_errors, this->count_not_null(), ramdon_moves);

  for(int i=1; i<9; i++){
    al_draw_line(TAM_BORDA + 1, TAM_BORDA + (TAM_CELULA * i), TAM_BORDA + (TAM_CELULA * 9) - 1, TAM_BORDA + (TAM_CELULA * i), al_map_rgb(50, 50, 50), 1.0);
    al_draw_line(TAM_BORDA + (TAM_CELULA * i), TAM_BORDA + 1, TAM_BORDA + (TAM_CELULA * i), TAM_BORDA + (TAM_CELULA * 9) - 1, al_map_rgb(50, 50, 50), 1.0);
  }

  for(int i=0; i<3; i++){
    for(int j=0; j<3; j++){
      al_draw_rectangle(TAM_BORDA + (TAM_CELULA * 3 * i), TAM_BORDA + (TAM_CELULA * 3 * j), TAM_BORDA + (TAM_CELULA * 3 * (i+1)), TAM_BORDA + (TAM_CELULA * 3 * (j+1)), al_map_rgb(50, 50, 50), 2.0);
    }
  }

  for(int i=0; i<9; i++){
    for(int j=0; j<9; j++){
      if(this->board[j][i].second.second && this->original[j][i]){
        al_draw_textf(fonte, al_map_rgb(0, 0, 100), TAM_BORDA + (TAM_CELULA * i) + TAM_CELULA/4, TAM_BORDA + (TAM_CELULA * j), 10, "%i", board[j][i].second.first);
      }else if(this->board[j][i].second.second){
        al_draw_textf(fonte, al_map_rgb(100, 0, 0), TAM_BORDA + (TAM_CELULA * i) + TAM_CELULA/4, TAM_BORDA + (TAM_CELULA * j), 10, "%i", board[j][i].second.first);
      }
    }
  }

  al_flip_display();
}
