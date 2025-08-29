/*
  SD CONNECTIONS (esp32 s3)
  Connect CS to pin 5
  Connect SCLK to pin 10
  Connect MISO  to pin 8
  Connect MOSI to pin 3
*/

//libraries
#include "SD.h"
#include "SD.h"
#include "SPI.h"

//other classes
#include "storage.hpp"
#include "console.hpp"
using namespace console;
using namespace storage;

//assign custom pins
#define REASSIGN_PINS
int sck = 10;
int miso = 8;
int mosi = 3;
int cs = 5;

namespace storage{

//start the sd (also runs a bunch of checks)
void begin(){
  #ifdef REASSIGN_PINS
  SPI.begin(sck, miso, mosi, cs);
  if (!SD.begin(cs)) {
  #else
  if (!SD.begin()) {
    #endif
    console::println("Card Mount Failed", 2);
    return;
  }
  uint8_t cardType = SD.cardType();

  if (cardType == CARD_NONE) {
    console::println("No SD card attached", 2);
    return;
  }

  console::print("SD Card Type: ", 0);
  if (cardType == CARD_MMC) {
    console::println("MMC", 0);
  } else if (cardType == CARD_SD) {
    console::println("SDSC", 0);
  } else if (cardType == CARD_SDHC) {
    console::println("SDHC", 0);
  } else {
    console::println("UNKNOWN", 1);
  }

  uint64_t cardSize = SD.cardSize() / (1024 * 1024);
  console::print("SD Card Size: ", 0);
  console::printi(cardSize, 0);
  console::enter();


  listDir("/", 0);
  createDir("/mydir");
  removeDir("/mydir");
  listDir("/", 2);
  writeFile("/hello.txt", "Hello ");
  appendFile("/hello.txt", "World!\n");
  readFile("/hello.txt");
  deleteFile("/foo.txt");
  renameFile("/hello.txt", "/foo.txt");
  readFile("/foo.txt");
}

//displays the chosen directory
void listDir(const char *dirname, uint8_t levels) {
  console::print("Listing directory: ", 0);
  console::println(dirname, 0);

  File root = SD.open(dirname);
  if (!root) {
    console::println("Failed to open directory", 2);
    return;
  }
  if (!root.isDirectory()) {
    console::println("Not a directory", 1);
    return;
  }

  File file = root.openNextFile();
  while (file) {
    if (file.isDirectory()) {
      console::print("  DIR : ", 0);
      console::println(file.name(), 0);
      if (levels) {
        listDir(file.path(), levels - 1);
      }
    } else {
      console::print("  FILE: ", 0);
      console::print(file.name(), 0);
      console::print("  SIZE: ", 0);
      console::printi(file.size(), 0);
      console::enter();
    }
    file = root.openNextFile();
  }
}

//creates a dir (basically mkdir)
void createDir(const char *path) {
  console::print("Creating Dir: ", 0);
  console::println(path, 0);
  if (SD.mkdir(path)) {
    console::println("Dir created", 0);
  } else {
    console::println("mkdir failed", 2);
  }
}

//removes a dir
void removeDir(const char *path) {
  console::print("Removing Dir: ", 0);
  console::print(path, 0);
  if (SD.rmdir(path)) {
    console::println("Dir removed", 1);
  } else {
    console::println("rmdir failed", 2);
  }
}

//read a file in ASCII (gonna change it later)
void readFile(const char *path) {
  console::print("Reading file: ", 0);
  console::println(path, 0);

  File file = SD.open(path);
  if (!file) {
    console::println("Failed to open file for reading", 2);
    return;
  }

  console::print("Read from file: ", 0);
  while (file.available()) {
    console::printi(file.read(), 0);
  }
  file.close();
}

//Writes something to a file, if it is not present, create one.
void writeFile(const char *path, const char *message) {
  console::print("Writing file: ", 0);
  console::println(path, 0);

  File file = SD.open(path, FILE_WRITE);
  if (!file) {
    console::println("Failed to open file for writing", 2);
    return;
  }
  if (file.print(message)) {
    console::println("File written", 0);
  } else {
    console::println("Write failed", 2);
  }
  file.close();
}

//basically write file (better use this)
void appendFile(const char *path, const char *message) {
  console::print("Appending to file: ", 0);
  console::println(path, 0);

  File file = SD.open(path, FILE_APPEND);
  if (!file) {
    console::println("Failed to open file for appending", 2);
    return;
  }
  if (file.print(message)) {
    console::println("Message appended", 0);
  } else {
    console::println("Append failed", 2);
  }
  file.close();
}

//renames a file with two directory names
void renameFile(const char *path1, const char *path2) {
  console::print("Renaming file ", 0);
  console::print(path1, 0);
  console::print(" to ", 0);
  console::println(path2, 0);
  if (SD.rename(path1, path2)) {
    console::println("File renamed", 0);
  } else {
    console::println("Rename failed", 2);
  }
}

//deletes a file
void deleteFile(const char *path) {
  console::print("Deleting file: ", 0);
  console::println(path, 0);
  if (SD.remove(path)) {
    console::println("File deleted", 1);
  } else {
    console::println("Delete failed", 2);
  }
}

/*
void testFileIO(const char *path) {
  File file = SD.open(path);
  static uint8_t buf[512];
  size_t len = 1;
  uint32_t start = millis();
  uint32_t end = start;
  if (file) {
    len = file.size();
    size_t flen = len;
    start = millis();
    while (len) {
      size_t toRead = len;
      if (toRead > 512) {
        toRead = 512;
      }
      file.read(buf, toRead);
      len -= toRead;
    }
    end = millis() - start;
    console::printf("%u bytes read for %lu ms\n", flen, end);
    file.close();
  } else {
    console::println("Failed to open file for reading");
  }

  file = SD.open(path, FILE_WRITE);
  if (!file) {
    console::println("Failed to open file for writing");
    return;
  }

  size_t i;
  start = millis();
  for (i = 0; i < 2048; i++) {
    file.write(buf, 512);
  }
  end = millis() - start;
  console::printf("%u bytes written for %lu ms\n", 2048 * 512, end);
  file.close();
}
*/
}