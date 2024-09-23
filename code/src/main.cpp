#include "SaveArray.hpp"
#include "SaveGameTest.hpp"
#include <iostream>

void readTest(SaveGameTest saveGame) {
  std::cout << "Level number: " << saveGame.getLevelNr() << std::endl;
  std::vector<unsigned> inventory = saveGame.getInventory();
  std::cout << "Inventory items:" << std::endl;
  for (unsigned item : inventory) {
    std::cout << item << std::endl;
  }
}

void writeTest(SaveGameTest saveGame) {
  saveGame.setLevelStatus("Miserable");
  saveGame.setPlayerLevel(0);
  saveGame.write();
}

int main() {
  SaveArray array{"anArray"};
  array.setField("aField", "aValue");
}
