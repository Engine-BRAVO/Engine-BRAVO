#include "SaveGame.hpp"
#include <fstream>
#include <iostream>
#include <nlohmann/json.hpp> // Include nlohmann/json

using json = nlohmann::json;

SaveGame::SaveGame(const std::string &aFileName) : mFileName(aFileName) {
  std::ifstream inFile(mFileName);
  if (inFile) {
    // File exists, load the data
    json j;
    inFile >> j;

    // Load fields from JSON
    if (j.contains("fields")) {
      for (const auto &field : j["fields"]) {
        if (field.contains("name") && field.contains("value")) {
          addAnyFromString(field["name"], field["value"]);
        }
      }
    }

    // Load arrays from JSON
    if (j.contains("arrays")) {
      for (const auto &array : j["arrays"]) {
        if (array.contains("name") && array.contains("fields")) {
          SaveArray saveArray(array["name"]);

          // Only add fields if they exist
          if (!array["fields"].empty()) {
            for (const auto &arrayField : array["fields"]) {
              if (arrayField.contains("name") && arrayField.contains("value")) {
                saveArray.addAnyFromString(arrayField["name"],
                                           arrayField["value"]);
              }
            }
          }

          mArrays.push_back(saveArray);
        }
      }
    }
  } else {
    // File doesn't exist, create a new one
    createFile();
  }
}

void SaveGame::store() {
  json j;

  // Serialize fields
  for (auto &field : mFields) {
    j["fields"].push_back(
        {{"name", field.getName()}, {"value", field.getValue()}});
  }

  // Serialize arrays
  for (auto &array : mArrays) {
    json arrayJson;
    arrayJson["name"] = array.getName();

    for (auto &field :
         array
             .getArray()) { // assuming public access to mFields or add a getter
      arrayJson["fields"].push_back(
          {{"name", field.getName()}, {"value", field.getValue()}});
    }
    j["arrays"].push_back(arrayJson);
  }

  // Write to file
  std::ofstream outFile(mFileName);
  if (outFile) {
    outFile << j.dump(4); // Pretty print with 4 spaces
  } else {
    std::cerr << "Failed to open the file: " << mFileName << std::endl;
  }
}

void SaveGame::addAnyFromString(std::string aName, std::string aValue) {
  if (isInteger(aValue)) {
    addIntField(aName, std::stoi(aValue));
  } else if (isFloat(aValue)) {
    addFloatField(aName, std::stof(aValue));
  } else {
    addStringField(aName, aValue);
  }
}

void SaveGame::addIntField(std::string aName, int aValue) {
  // Check if the field already exists
  try {
    for (auto &field : mIntFields) {
      if (field.getName() == aName) {
        throw(aName);
      }
    }
  } catch (std::string val) {
    std::cout << "SaveGame::addIntField(): cannot add field with name \""
              << aName << "\", field already exists." << std::endl;
    return;
  }

  mIntFields.emplace_back(aName, aValue);
}

void SaveGame::addFloatField(std::string aName, float aValue) {
  // Check if the field already exists
  try {
    for (auto &field : mFloatFields) {
      if (field.getName() == aName) {
        throw(aName);
      }
    }
  } catch (std::string val) {
    std::cout << "SaveGame::addFloatField(): cannot add field with name \""
              << aName << "\", field already exists." << std::endl;
    return;
  }

  mFloatFields.emplace_back(aName, aValue);
}

void SaveGame::addStringField(std::string aName, std::string aValue) {
  // Check if the field already exists
  try {
    for (auto &field : mStringFields) {
      if (field.getName() == aName) {
        throw(aName);
      }
    }
  } catch (std::string val) {
    std::cout << "SaveGame::addStringField(): cannot add field with name \""
              << aName << "\", field already exists." << std::endl;
    return;
  }

  mStringFields.emplace_back(aName, aValue);
}

void SaveGame::remove() { std::remove(mFileName.c_str()); }

IntSaveField &SaveGame::getIntField(std::string aName) const {
  for (IntSaveField field : mIntFields) {
    if (field.getName() == aName) {
      return field;
    }
  }
  throw("Failed to get field " + aName);
}

FloatSaveField &SaveGame::getFloatField(std::string aName) const {
  for (FloatSaveField field : mFloatFields) {
    if (field.getName() == aName) {
      return field;
    }
  }
  throw("Failed to get field " + aName);
}

StringSaveField &SaveGame::getStringField(std::string aName) const {
  for (StringSaveField field : mStringFields) {
    if (field.getName() == aName) {
      return field;
    }
  }
  throw("Failed to get field " + aName);
}

void SaveGame::addArray(std::string aName) {
  // Check if the array already exists
  try {
    for (auto &array : mArrays) {
      if (array.getName() == aName) {
        throw(aName);
      }
    }
  } catch (std::string val) {
    std::cout << "SaveGame::addArray(): cannot add array with name \"" << aName
              << "\", field already exists." << std::endl;
    return;
  }

  SaveArray array(aName);
  mArrays.push_back(array);
}

void SaveGame::setArray(std::string aName, SaveArray aValue) {
  try {
    for (SaveArray &array : mArrays) {
      if (array.getName() == aName) {
        array = aValue;
        return;
      }
    }
    throw(aName);
  } catch (std::string aName) {
    std::cout << "SaveArray::setArray(): failed to find array with name \""
              << aName << "\"" << std::endl;
    return;
  }
}

SaveArray SaveGame::getArray(std::string aName) const {
  try {
    for (SaveArray array : mArrays) {
      if (array.getName() == aName) {
        return array;
      }
    }
    throw(aName);
  } catch (std::string aName) {
    std::cout << "SaveArray::getField(): failed to find field with name \""
              << aName << "\"" << std::endl;
    return mArrays[0];
  }
}

void SaveGame::createFile() {
  std::ofstream outFile(mFileName);
  if (!outFile) {
    std::cerr << "Failed to create the file: " << mFileName << std::endl;
  }
}

bool SaveGame::isInteger(const std::string &s) const {
  std::istringstream iss(s);
  int val;
  // Attempt to read an integer from the string
  iss >> std::noskipws >> val;
  // Check if the entire string was used (iss should be fully consumed)
  return iss.eof() && !iss.fail();
}

bool SaveGame::isFloat(const std::string &s) const {
  std::istringstream iss(s);
  float val;
  // Attempt to read a float from the string
  iss >> std::noskipws >> val;
  // Check if the entire string was used (iss should be fully consumed)
  return iss.eof() && !iss.fail();
}