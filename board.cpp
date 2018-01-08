#include "board.hpp"
#include <stdlib.h>

Board::Board(){
  this->valid = true;
}
Board::~Board(){}

void Board::read_board(std::string file_name){
  FILE* in = fopen(file_name.c_str(), "r");
  char line[11];
  for(int i = 0; i < 9; i++){
    bool result = fgets(line, 11, in);
    if(!result){
      exit(1);
    }
    std::vector<std::pair<std::vector<int>,std::pair<int,bool>>> aux;
    std::vector<bool> aux_original;
    for(int j = 0; j < 9; j++){
      std::pair<std::vector<int>,std::pair<int,bool>> element;
      element.second.first = line[j] - 48;
      if(element.second.first == 0){
        element.second.second = false;
        aux_original.push_back(false);
      }else{
        element.second.second = true;
        aux_original.push_back(true);
      }
      aux.push_back(element);
    }
    this->original.push_back(aux_original);
    this->board.push_back(aux);
  }
}

void Board::init_potential(){
  for(int i=0; i<9; i++){
    for(int j=0; j<9; j++){
      if(this->board[i][j].second.second == false){
        this->board[i][j].first = this->potential_verify(i,j);
      }
    }
  }
}

std::vector<int> Board::potential_verify(int x, int y){
  std::vector<int> potential;
  int mirror[] = {1,2,3,4,5,6,7,8,9};
  for(int i=0; i<3; i++){
    for(int j=0; j<3; j++){
      if(this->board[i+(x/3)*3][j+(y/3)*3].second.second){
        mirror[this->board[i+(x/3)*3][j+(y/3)*3].second.first-1] = 0;
      }
    }
  }
  for(int i=0; i<9; i++){
    if(this->board[i][y].second.second){
      mirror[this->board[i][y].second.first-1] = 0;
    }
  }
  for(int j=0; j<9; j++){
    if(this->board[x][j].second.second){
      mirror[this->board[x][j].second.first-1] = 0;
    }
  }
  for(int i=0; i<9; i++){
    if(mirror[i]!=0){
      potential.push_back(mirror[i]);
    }
  }
  return potential;
}

void Board::put_the_single_potential(){
  bool swap = true;
  while(swap){
    swap = false;
    for(int i=0; i<9; i++){
      for(int j=0; j<9; j++){
        if(this->board[i][j].second.second == false){
          if(this->board[i][j].first.size() == 1){
            this->board[i][j].second.first = this->board[i][j].first[0];
            this->board[i][j].second.second = true;
            // this->board[i][j].first.pop_back();
            this->fix_potential(i,j);
            this->show_board();
            swap = true;
          }else{
            bool aux = this->verify_when_just_one_empyt_have_the_element(i, j);
            if(aux){
              this->show_board();
            }
            swap = swap || aux;
          }
        }
      }
    }
  }
}

void Board::fix_potential(int x, int y){
  std::vector<int> potential;
  for(int i=0; i<3; i++){
    for(int j=0; j<3; j++){
      if(this->board[i+(x/3)*3][j+(y/3)*3].second.second == false){
        this->board[i+(x/3)*3][j+(y/3)*3].first = this->remove_element(i+(x/3)*3, j+(y/3)*3, this->board[x][y].second.first);
      }
    }
  }
  for(int i=0; i<9; i++){
    if(this->board[i][y].second.second == false){
      this->board[i][y].first = this->remove_element(i, y, this->board[x][y].second.first);
    }
  }
  for(int j=0; j<9; j++){
    if(this->board[x][j].second.second == false){
      this->board[x][j].first = this->remove_element(x, j, this->board[x][y].second.first);
    }
  }
}

std::vector<int> Board::remove_element(int x, int y, int element){
  std::vector<int> new_vector;
  for(int i=0; i< (int) this->board[x][y].first.size(); i++){
    if(this->board[x][y].first[i] != element){
      new_vector.push_back(this->board[x][y].first[i]);
      // return (this->board[x][y].first).erase((this->board[x][y].first).begin()+(i+1));
    }
  }
  return new_vector;
}

bool Board::verify_when_just_one_empyt_have_the_element(int x, int y){
  for(int k=0; k< (int) this->board[x][y].first.size(); k++){
    bool alone = true;
    for(int i=0; i<3; i++){
      for(int j=0; j<3; j++){
        if(this->board[i+(x/3)*3][j+(y/3)*3].second.second == false && (x!=i || y!=j)){
          if(element_in_vector(this->board[x][y].first[k], this->board[i+(x/3)*3][j+(y/3)*3].first)){
            alone = false;
          }
        }
      }
    }
    if(alone){
      this->board[x][y].second.first = this->board[x][y].first[k];
      this->board[x][y].second.second = true;
      // this->board[x][y].first.clear();
      this->fix_potential(x,y);
      return true;
    }
    alone = true;
    for(int i=0; i<9; i++){
      if(this->board[i][y].second.second == false || x!=i){
        if(element_in_vector(this->board[x][y].first[k], this->board[i][y].first)){
          alone = false;
        }
      }
    }
    if(alone){
      this->board[x][y].second.first = this->board[x][y].first[k];
      this->board[x][y].second.second = true;
      // this->board[x][y].first.clear();
      this->fix_potential(x,y);
      return true;
    }
    alone = true;
    for(int j=0; j<9; j++){
      if(this->board[x][j].second.second == false || y!=j){
        if(element_in_vector(this->board[x][y].first[k], this->board[x][j].first)){
          alone = false;
        }
      }
    }
    if(alone){
      this->board[x][y].second.first = this->board[x][y].first[k];
      this->board[x][y].second.second = true;
      // this->board[x][y].first.clear();
      this->fix_potential(x,y);
      return true;
    }
  }
  return false;
}

bool Board::element_in_vector(int element, std::vector<int> vector){
  for(int i=0; i<(int)vector.size(); i++){
    if(vector[i] == element){
      return true;
    }
  }
  return false;
}

bool Board::move_wrong(){
  for(int i=0; i<9; i++){
    for(int j=0; j<9; j++){
      if(this->board[i][j].second.second == false && this->board[i][j].first.empty()){
        // std::cout << "ruim" << std::endl;
        return true;
      }
    }
  }
  return false;
}

bool Board::end_game(){
  for(int i=0; i<9; i++){
    for(int j=0; j<9; j++){
      if(this->board[i][j].second.second == false){
        return false;
      }
    }
  }
  std::cout << "end game" << std::endl;
  return true;
}

void Board::put_ramdon_element(){
  int i = rand() % 9;
  int j = rand() % 9;
  if(this->board[i][j].second.second){
    return;
  }
  int k = rand() % (int) this->board[i][j].first.size();
  this->board[i][j].second.first = this->board[i][j].first[k];
  this->board[i][j].second.second = true;
  this->fix_potential(i,j);
}

int Board::count_not_null(){
  int count = 0;
  for(int i=0; i<9; i++){
    for(int j=0; j<9; j++){
      if(this->board[i][j].second.second){
        count++;
      }
    }
  }
  return count;
}
