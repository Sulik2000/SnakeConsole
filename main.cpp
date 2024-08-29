#include <iostream>
#include <utility>
#include <vector>
#include <time.h>

#define WIDTH_FIELD 10
#define HEIGHT_FIELD 10

std::vector<std::pair<int, int> > appleCells;


bool CheckCell(std::vector<std::pair<int, int> > cells, int x, int y){
  for(auto a : cells){
    if(a.first == x && a.second == y)
      return true;
  }

  return false;
}

enum class Direction{
  Right = 1,
  Top = 2,
  Left = 3,
  Bottom = 4
};

class Snake{
  std::vector<std::pair<int, int> > _cells;
  Direction _direction = Direction::Right;
public:
  Snake(){
    std::pair<int, int> temp(WIDTH_FIELD/2, HEIGHT_FIELD/2);
    _cells.push_back(temp);
  }
  std::vector<std::pair<int, int> > GetCells(){
    return _cells;
  }

  void SpawnCell(){
    int x = _cells[_cells.size() - 1].first, y = _cells[_cells.size() - 1].second;

    switch(_direction){
      case 1:
        x--;
        break;
      case 2:
        y++;
        break;
      case 3:
        x++;
        break;
      case 4:
        y--;
        break;
    }

    _cells.push_back(std::pair<int, int>(x, y));
  }

  void EatApple(){
    if(CheckCell(appleCells, _cells[0].first, _cells[0].second)){
      for(int i = 0; i < appleCells.size(); i++){
        if(appleCells[i] == _cells[0]){
          appleCells.erase(appleCells.begin() + i);
        }
      }
      SpawnCell();
    }
  }
  void GameCondition(){
    for(int i = 1; i < _cells.size(); i++){
      if(_cells[i] == _cells[0]){
        std::cout << "\n\n\n\n\n----------DEFEAT----------";
        exit(0);
      }
    }

    if(_cells.size() == WIDTH_FIELD * HEIGHT_FIELD){
      std::cout << "\n\n\n\n\n----------VICTORY----------";
      exit(0);
    }
  }
  void Frame(){
    int nextX = _cells[0].first, nextY = _cells[0].second;

    switch(_direction){
      case 1:
        nextX++;
        break;
      case 2:
        nextY--;
        break;
      case 3:
        nextX--;
        break;
      case 4:
        nextY++;
        break;
    }
    if(nextX >= WIDTH_FIELD)
        nextX = 0;
      else if(nextX < 0)
        nextX = WIDTH_FIELD - 1;
      if(nextY >= HEIGHT_FIELD)
        nextY = 0;
      else if(nextY < 0)
        nextY = HEIGHT_FIELD - 1;

    for(int i = 0; i < _cells.size(); i++){
      int bufX = _cells[i].first, bufY = _cells[i].second;
      
      _cells[i] = std::pair<int, int>(nextX, nextY);
      nextX = bufX;
      nextY = bufY;  
    }
    EatApple();

    GameCondition();
  }
  void SetDirection(char a){
    switch(a){
      case 'd':
        _direction = Direction::Right;
        break;
      case 'a':
        _direction = Direction::Left;
        break;

      case 'w':
        _direction = Direction::Top;
        break;
      case 's':
        _direction = Direction::Bottom;
        break;
      default:
        _direction = Direction::Right;
        break;
    }
  }
};

void DrawField(std::vector<std::pair<int, int> > cells, std::vector<std::pair<int, int> > appleCells){
  for(int i = 0; i < HEIGHT_FIELD; i++){
    for(int j = 0; j < WIDTH_FIELD; j++){
      if(CheckCell(appleCells, j, i))
        std::cout << 'A';
      else if(CheckCell(cells, j, i))
        std::cout << '#';
      else
        std::cout << '.';
    }
    std::cout << '\n';
  }
}

std::vector<std::pair<int, int> > SpawnApples(std::vector<std::pair<int, int> > cells, int count){
  std::vector<std::pair<int, int> > result;

  for(int i = 0; i < count; i++){
    int cellID = (rand() % (WIDTH_FIELD * HEIGHT_FIELD)) - cells.size();

    for(int k = 0; k < HEIGHT_FIELD > 0 && cellID > 0; k++){
      for(int j = 0; j < WIDTH_FIELD && cellID > 0; j++){
        if(!CheckCell(cells, j, k))
          cellID--;

        if(cellID == 0){
          result.push_back(std::pair<int, int>(j, k));
        }
      }
    }
  }

  return result;
}

int main() {
  int countOfApples = 2;
  srand(time(NULL));
  Snake* snake = new Snake();
  appleCells = SpawnApples(snake->GetCells(), countOfApples);
  // Game cycle
  while(true){
    std::cout << "Enter snake direction: ";
    
    char s = ' ';
    std::cin >> s;

    snake->SetDirection(s);
    
    snake->Frame();

    auto cells = snake->GetCells();

    if(appleCells.empty())
      appleCells = SpawnApples(snake->GetCells(), countOfApples);
    
    DrawField(cells, appleCells);
  }
  return 0;
}
