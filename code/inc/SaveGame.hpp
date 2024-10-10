#pragma once

#include "SaveArray.hpp"
#include "SaveField.hpp"
#include <string>

class SaveGame {
public:
  SaveGame(const std::string &aFileName);

public:
  void createFile();
  void store();
  void remove();

public:
  void addField(std::string name, std::string value);
  void setField(std::string name, std::string value);
  std::string getField(std::string aName) const;

public:
  void addArray(std::string name);
  void setArray(std::string aName, SaveArray aValue);
  SaveArray getArray(std::string aName) const;

private:
  std::string mFileName;
  std::vector<SaveField<std::string>> mFields;
  std::vector<SaveArray> mArrays;
};