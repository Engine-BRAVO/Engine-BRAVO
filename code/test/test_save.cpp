#include "FSConverter.h"
#include "SaveGame.h"
#include <fstream>
#include <gtest/gtest.h>

TEST(IntSaveField, settingGetting) {
    IntSaveField intSaveField{"int1"};
    intSaveField.setValue(1);
    ASSERT_EQ(intSaveField.getValue(), 1);
    intSaveField.setValue(2);
    ASSERT_EQ(intSaveField.getValue(), 2);
}

TEST(SaveGame, ValidWriteRead) {
    FSConverter fsConverter;
    std::string path = fsConverter.getResourcePath("saves/newSave.json");
    std::remove(path.c_str());
    SaveGame sg{path};
    sg.addStringField("string1", "value1");
    sg.addIntField("int1", 1);
    sg.addFloatField("float1", 1.0f);

    ASSERT_FLOAT_EQ(sg.getFloatField("float1").getValue(), 1.0f);

    sg.setFloatField("float1", 1.1f);

    ASSERT_EQ(sg.getStringField("string1").getValue(), "value1");
    ASSERT_EQ(sg.getIntField("int1").getValue(), 1);
    ASSERT_FLOAT_EQ(sg.getFloatField("float1").getValue(), 1.1f);
}

TEST(SaveGame, InvalidWriteRead) {
    FSConverter fsConverter;
    std::string path = fsConverter.getResourcePath("saves/newSave.json");
    std::remove(path.c_str());
    SaveGame sg{path};

    ASSERT_THROW(sg.getStringField("string1"), std::invalid_argument);
    ASSERT_THROW(sg.getIntField("int1"), std::invalid_argument);
    ASSERT_THROW(sg.getFloatField("float1"), std::invalid_argument);

    ASSERT_THROW(sg.setStringField("string1", "value1"), std::invalid_argument);
    ASSERT_THROW(sg.setIntField("int1", 1), std::invalid_argument);
    ASSERT_THROW(sg.setFloatField("float1", 1.0f), std::invalid_argument);

    ASSERT_THROW(sg.setArray("array1", SaveArray("array1name")), std::invalid_argument);
}

TEST(SaveGame, Store) {
    FSConverter fsConverter;
    std::string path = fsConverter.getResourcePath("saves/newSave.json");
    std::remove(path.c_str());
    SaveGame sg{path};

    sg.addIntField("int1", 1);
    sg.addFloatField("float1", 1.0f);
    sg.addArray("array1");
    SaveArray array{sg.getArray("array1")};
    array.addStringField("string1", "value1");
    sg.setArray("array1", array);
    sg.store();

    // Check if the file contents are correct
    std::ifstream file(path);
    ASSERT_TRUE(file.is_open());

    std::string fileContent((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
    std::string expectedContent = "{\n"
                                  "    \"arrays\": [\n"
                                  "        {\n"
                                  "            \"fields\": [\n"
                                  "                {\n"
                                  "                    \"name\": \"string1\",\n"
                                  "                    \"value\": \"value1\"\n"
                                  "                }\n"
                                  "            ],\n"
                                  "            \"name\": \"array1\"\n"
                                  "        }\n"
                                  "    ],\n"
                                  "    \"fields\": [\n"
                                  "        {\n"
                                  "            \"name\": \"int1\",\n"
                                  "            \"value\": 1\n"
                                  "        },\n"
                                  "        {\n"
                                  "            \"name\": \"float1\",\n"
                                  "            \"value\": 1.0\n"
                                  "        }\n"
                                  "    ]\n"
                                  "}";

    file.close();

    ASSERT_EQ(expectedContent, fileContent);

    std::remove(path.c_str());
}

TEST(SaveGame, InvalidStore) {
    FSConverter fsConverter;
    std::string path = fsConverter.getResourcePath("invalid/newSave.json");
    std::remove(path.c_str());
    SaveGame sg{path};

    sg.addIntField("int1", 1);
    sg.addFloatField("float1", 1.0f);
    sg.addArray("array1");
    SaveArray array{sg.getArray("array1")};
    array.addStringField("string1", "value1");
    sg.setArray("array1", array);
    ASSERT_THROW(sg.store(), std::invalid_argument);

    // Check if the file contents are correct
    std::ifstream file(path);
    ASSERT_FALSE(file.is_open());

    std::remove(path.c_str());
}

TEST(SaveGame, Remove) {
    FSConverter fsConverter;
    std::string path = fsConverter.getResourcePath("saves/newSave.json");
    std::remove(path.c_str());
    SaveGame sg{path};

    sg.addIntField("int1", 1);

    sg.store();
    sg.remove();

    // Check if the file was removed
    std::ifstream file(path);
    ASSERT_FALSE(file.is_open());
}

TEST(SaveGame, InvalidRemove) {
    FSConverter fsConverter;
    std::string path = fsConverter.getResourcePath("saves/newSave.json");
    std::remove(path.c_str());
    SaveGame sg{path};

    sg.addIntField("int1", 1);

    EXPECT_THROW(sg.remove(), std::logic_error);
}

TEST(SaveArray, WriteRead) {

    FSConverter fsConverter;
    std::string path = fsConverter.getResourcePath("saves/newSave.json");
    std::remove(path.c_str());
    SaveGame sg{path};

    sg.addArray("array1");
    SaveArray array{sg.getArray("array1")};
    array.addIntField("int1", 1);
    array.addFloatField("float1", 1.0f);
    array.addStringField("string1", "value1");
    sg.setArray("array1", array);

    SaveArray array2{sg.getArray("array1")};
    ASSERT_EQ(array2.getIntField("int1").getValue(), 1);
    ASSERT_FLOAT_EQ(array2.getFloatField("float1").getValue(), 1.0f);
    ASSERT_EQ(array2.getStringField("string1").getValue(), "value1");
}