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
  std::vector<double> rough_vector;
  std::fstream file_stream(file_path);
  std::string line;
  // First four lines go in as is
  while (std::getline(file_stream, line)){
    rough_vector.push_back(std::stod(line) * -1);
  }
  std::vector<double> smooth_vector(rough_vector.size());
  //Add first four from rough
  for (int i=0; i < 3; i++) {
    smooth_vector[i] = (rough_vector[i]);
  }
  //apply weighted average to middle
  for (int i=3; i < rough_vector.size() -3; i++) {
    smooth_vector[i] = ((rough_vector[i - 3] +
    2*rough_vector[i - 2] +
    3*rough_vector[i - 1] +
    3*rough_vector[i] +
    3*rough_vector[i + 1] +
    2*rough_vector[i + 2] +
    rough_vector[i + 3]) / 15);
  }
  //Add last three to smoothed vector
  for (int i=rough_vector.size() - 3; i < rough_vector.size(); i++) {
    smooth_vector[i] = (rough_vector[i]);
  }
  // RVO should move to avoid an unnesicary copy
  //test prints:
  for (size_t i = 0; i < rough_vector.size(); ++i) {
    std::cout << rough_vector[i] << " \t " << smooth_vector[i] << std::endl;
  }

  return smooth_vector;
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
      smooth_data(file);
    }
  } else {
    std::cerr << "ERROR: directory not found at" << dir_path;
  }
  
  return 1;
}
