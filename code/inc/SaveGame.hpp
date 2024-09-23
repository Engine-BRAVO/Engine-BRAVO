#pragma once

#include "SaveArray.hpp"
#include "SaveField.hpp"
#include <string>

class SaveGame {
public:
  SaveGame(std::string aFileName);

public:
  void store();

public:
  void addField(std::string name, std::string value);
  void setField(std::string name, std::string value);
  std::string getField(std::string aName);

public:
  void addArray(std::string name);
  const SaveArray &getArray(std::string aName);

private:
  std::string mFileName;
  std::vector<SaveField> mFields;
  std::vector<SaveArray> mArrays;
};