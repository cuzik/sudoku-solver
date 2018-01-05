#include <iostream>
#include <vector>
#include <string>

class Board{
public:
  std::vector<std::vector<std::pair<std::vector<int>,std::pair<int,bool>>>> board;
  std::vector<std::vector<bool>> original;
  bool valid;

  Board();
  ~Board();
  void read_board(std::string file_name);
  void show_board();
  void init_potential();
  void put_the_single_potential();
  bool move_wrong();
  bool end_game();
  void put_ramdon_element();
  int count_not_null();

private:
  bool verify_when_just_one_empyt_have_the_element(int x, int y);
  std::vector<int> potential_verify(int x, int y);
  void fix_potential(int x, int y);
  std::vector<int> remove_element(int x, int y, int element);
  bool element_in_vector(int element, std::vector<int> vector);
};
