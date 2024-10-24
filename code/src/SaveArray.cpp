#include "SaveArray.hpp"
#include "FloatSaveField.hpp"
#include "IntSaveField.hpp"
#include "SaveGame.hpp"
#include "SaveGameUtil.hpp"
#include "StringSaveField.hpp"
#include <iostream>

SaveArray::SaveArray(std::string name) : mName(name) {}

std::string SaveArray::getName() const { return mName; }

void SaveArray::addAnyFromString(std::string aName, std::string aValue) {
  if (SaveGameUtil::isInteger(aValue)) {
    addIntField(aName, std::stoi(aValue));
  } else if (SaveGameUtil::isFloat(aValue)) {
    addFloatField(aName, std::stof(aValue));
  } else {
    addStringField(aName, aValue);
  }
}

void SaveArray::addIntField(std::string aName, int aValue) {
  // Check if the field already exists
  for (auto &field : mIntFields) {
    if (field.getName() == aName) {
      std::cout << "SaveArray::addIntField(): cannot add field with name \""
                << field.getName()
                << "\", field already exists. Overwriting value instead"
                << std::endl;
      field.setValue(aValue);
      return;
    }
  }

  mIntFields.emplace_back(aName, aValue);
}

void SaveArray::addFloatField(std::string aName, float aValue) {
  // Check if the field already exists
  for (auto &field : mFloatFields) {
    if (field.getName() == aName) {
      std::cout << "SaveArray::addFloatField(): cannot add field with name \""
                << field.getName()
                << "\", field already exists. Overwriting value instead"
                << std::endl;
      field.setValue(aValue);
      return;
    }
  }

  mFloatFields.emplace_back(aName, aValue);
}

void SaveArray::addStringField(std::string aName, std::string aValue) {
  // Check if the field already exists
  for (auto &field : mStringFields) {
    if (field.getName() == aName) {
      std::cout << "SaveArray::addStringField(): cannot add field with name \""
                << field.getName()
                << "\", field already exists. Overwriting value instead"
                << std::endl;
      field.setValue(aValue);
      return;
    }
  }

  mStringFields.emplace_back(aName, aValue);
}

IntSaveField &SaveArray::getIntField(std::string aName) {
  for (IntSaveField &field : mIntFields) {
    if (field.getName() == aName) {
      return field;
    }
  }
  throw("Failed to get field " + aName);
}

FloatSaveField &SaveArray::getFloatField(std::string aName) {
  for (FloatSaveField &field : mFloatFields) {
    if (field.getName() == aName) {
      return field;
    }
  }
  throw("Failed to get field " + aName);
}

StringSaveField &SaveArray::getStringField(std::string aName) {
  for (StringSaveField &field : mStringFields) {
    if (field.getName() == aName) {
      return field;
    }
  }
  throw("Failed to get field " + aName);
}

const std::vector<IntSaveField> &SaveArray::getIntArray() const {
  return mIntFields;
}

const std::vector<FloatSaveField> &SaveArray::getFloatArray() const {
  return mFloatFields;
}

const std::vector<StringSaveField> &SaveArray::getStringArray() const {
  return mStringFields;
}