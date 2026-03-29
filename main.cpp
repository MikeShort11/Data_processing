// Michael Short
// CS 3370 Project 4 - Data Processing with C++ Algorithms
// Utah Valley University - Todd Flyr

#include <csignal>
#include <cstdio>
#include <filesystem>
#include <iostream>
#include <fstream>
#include <stdexcept>
#include <string>
#include <vector>

namespace fs = std::filesystem;

struct params {
    double vt = 0.0;
    double width = 0.0;
    double pulse_delta = 0.0;
    double drop_ratio = 0.0;
    double below_drop_ratio = 0.0;
};

params parameters;

void read_ini(fs::path ini_path){
  //Bools to ensure data is all set
  bool vt_set = false;
  bool width_set = false;
  bool pulse_delta_set = false;
  bool drop_ratio_set = false;
  bool below_drop_ratio_set = false;
  //needed vars
  std::fstream ini_stream(ini_path);
  std::string line;
  std::string par;

  while (getline(ini_stream, line, '=')) {
    if (!getline(ini_stream, par)) break;
    line.erase(0, line.find_first_not_of(" \n\r\t"));
    line.erase(line.find_last_not_of(" \n\r\t") + 1);
    if (line == "vt") {
      parameters.vt = std::stod(par);
      vt_set = true;
    }
    else if (line == "width") {
      parameters.width = std::stod(par);
      width_set = true;
    }
    else if (line == "pulse_delta") {
      parameters.pulse_delta = std::stod(par);
      pulse_delta_set = true;
    }
    else if (line == "drop_ratio") {
      parameters.drop_ratio = std::stod(par);
      drop_ratio_set = true;
    }
    else if (line == "below_drop_ratio") {
      parameters.below_drop_ratio = std::stod(par);
      below_drop_ratio_set = true;
    }
    else {
      if (line[0] == '#' || line.empty()){
        continue;
      } else {
      throw std::runtime_error(line + " is not a valid option");
      }
    }
  }
  // Check that all vallues were set
  if (!(vt_set && width_set && pulse_delta_set && drop_ratio_set && below_drop_ratio_set)){
    throw std::runtime_error("All values must be set in .ini file");
  }
  //test print
  std::cout << "vt: " << parameters.vt << std::endl
  << "Width: "<< parameters.width << std::endl
  << "pulse delta: " << parameters.pulse_delta << std::endl
  << "drop ratio: " << parameters.drop_ratio << std::endl
  << "below drop ratio: " << parameters.below_drop_ratio << std::endl;
  }


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
  // for (size_t i = 0; i < rough_vector.size(); ++i) {
  //   std::cout << rough_vector[i] << " \t " << smooth_vector[i] << std::endl;
  // }

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

int main(int argc, char* argv[]) {
  if (argc != 2){
    std::cerr << "Usage: " << argv[0] << " <path_to_ini_file>" << std::endl;
    return 1;
  }

  fs::path ini_path = argv[1];
  fs::path dir_path = "./data_files";

  read_ini(ini_path);
  if (fs::exists(dir_path) && fs::is_directory(dir_path)){
    for (auto &file : fs::directory_iterator(dir_path)){
      if (file.path().extension() == ".dat") {
        smooth_data(file);
      }
    }
  } else {
    std::cerr << "ERROR: directory not found at" << dir_path;
  }
  
  return 1;
}
