// Michael Short
// CS 3370 Project 4 - Data Processing with C++ Algorithms
// Utah Valley University - Todd Flyr

#include <csignal>
#include <filesystem>
#include <iostream>
#include <fstream>
#include <stdexcept>
#include <string>
#include <vector>

namespace fs = std::filesystem;

std::vector<double> smooth_data(fs::path file_path) {
  //ERROR checks
  if (!fs::exists(file_path) || !fs::is_regular_file(file_path)) {
    throw std::runtime_error("ERROR: file not found at:" + file_path.string());
  }
  // A little redundant, but give better error messages
  if (file_path.extension() != ".dat") {
    throw std::runtime_error("ERROR: unsupported file type: " + file_path.extension().string());
  }

  //create a vector to return and needed vars
  std::vector<double> storage_vector;
  std::fstream file_stream(file_path);
  std::string line;
  // First four lines go in as is
  for (auto i=0; i<=4; i++) {
    std::getline(file_stream, line);
    storage_vector.push_back(std::stod(line));
  }
  while (std::getline(file_stream, line)){
  //TODO: fill out the smoothing algorithim  
  }
  // RVO should do this but i want to be explicit
  return std::move(storage_vector);
}

//testing function, MAKE SURE TO REMOVE AFTER NO LONGER NEEDED
void printfile(fs::path file_path){
  std::fstream fst(file_path);
  std::string line;
  while (std::getline(fst, line)){
        std::cout << line << std::endl;
  }
}

int main() {
  fs::path dir_path = "./data_files";
  if (fs::exists(dir_path) && fs::is_directory(dir_path)){
    for (auto &file : fs::directory_iterator(dir_path)){
      printfile(file);
    }
  } else {
    std::cerr << "ERROR: directory not found at" << dir_path;
  }
  
  return 1;
}
