// Try adding normal vectors to input data.

// Data from http://www.icst.pku.edu.cn/zlian/shrec15-non-rigid/index.htm
// 50 classes, 24 exemplars per class: alien ants armadillo bird1 bird2 camel
// cat centaur twoballs dinosaur dog1 dog2 glasses gorilla hand horse lamp paper
// man octopus pliers rabbit santa scissor shark snake spider dino_ske flamingo
// woman Aligator Bull Chick Deer Dragon Elephant Frog Giraffe Kangaroo Mermaid
// Mouse Nunchaku MantaRay Ring Robot Sumotori Tortoise Watch Weedle Woodman

#include "SpatiallySparseDatasetModelNet.h"
#include <vector>
#include <iostream>
#include <fstream>
#include <string>
#include <cstring>
#include "OpenCVPicture.h"
#include "Off3DFormatPicture.h"
#include <dirent.h>

std::vector<std::string> get_classes(const char* path) {
  // get list of all folders - names of classes
  DIR *dir = opendir(path);
  std::vector<std::string> classes_names;
  struct dirent *entry = readdir(dir);

  while (entry != NULL)
  {
    if (entry->d_type == DT_DIR) {
      if ((strcmp(entry->d_name, ".") != 0) && (strcmp(entry->d_name, "..") != 0)) {
        // std::cout << entry->d_name << std::endl;
        classes_names.push_back(entry->d_name);
      }
      entry = readdir(dir);
    }
  }
  closedir(dir);
  return classes_names;
}

std::vector<std::string> get_files_names(const char* path) {
  // get list of all folders - names of classes
  DIR *dir = opendir(path);
  std::vector<std::string> files_names;
  struct dirent *entry = readdir(dir);

  while (entry != NULL)
  {
    if (strstr(entry->d_name, ".off") != NULL) {
      // std::cout << entry->d_name << std::endl;
      files_names.push_back(entry->d_name);
    }
    entry = readdir(dir);
  }
  closedir(dir);
  return files_names;
}


SpatiallySparseDataset ModelNetTrainSet(int renderSize, int kFold, int fold) {
  SpatiallySparseDataset dataset;
  dataset.name = "ModelNet (Train subset)";
  dataset.type = TRAINBATCH;
  dataset.nFeatures = 1;
  int nClasses, nTotal;

  const char* PATH = "/media/toshiba/shape_retrieval_datasets/ModelNet/ModelNet40/";
  std::vector<std::string> classes_names = get_classes(PATH);
  nClasses = classes_names.size();

  dataset.nClasses = nClasses;

  std::cout << "Number of classes: " << nClasses << std::endl;

  for (int class_idx = 0; class_idx < nClasses; ++class_idx) {
    std::string className = classes_names[class_idx];

    std::string picture_path(PATH, strlen(PATH));
    picture_path = picture_path + className + std::string("/train/");

    std::cout << "Reading files from " << picture_path << std::endl;

    std::vector<std::string> class_pictures = get_files_names(picture_path.c_str());

    for (int exemp = 0; exemp < class_pictures.size(); exemp++) {
      int num;
      std::string filename = picture_path + class_pictures[exemp];
      if (exemp % kFold != fold) {
        dataset.pictures_path.push_back(filename);
        dataset.labels.push_back(class_idx);
      }
    }
  }
  dataset.pictures.resize(dataset.labels.size(), NULL);
  return dataset;
};

SpatiallySparseDataset ModelNetTestSet(int renderSize, int kFold, int fold) {
  SpatiallySparseDataset dataset;
  dataset.name = "ModelNet (Validation subset)";
  dataset.type = TESTBATCH;
  dataset.nFeatures = 1;
  int nClasses, nTotal;

  const char* PATH = "/media/toshiba/shape_retrieval_datasets/ModelNet/ModelNet40/";
  std::vector<std::string> classes_names = get_classes(PATH);
  nClasses = classes_names.size();

  dataset.nClasses = nClasses;

  std::cout << "Number of classes: " << nClasses << std::endl;

  for (int class_idx = 0; class_idx < nClasses; ++class_idx) {
    std::string className = classes_names[class_idx];

    std::string picture_path(PATH, strlen(PATH));
    picture_path = picture_path + className + std::string("/train/");

    std::cout << "Reading files from " << picture_path << std::endl;

    std::vector<std::string> class_pictures = get_files_names(picture_path.c_str());

    for (int exemp = 0; exemp < class_pictures.size(); exemp++) {
      int num;
      std::string filename = picture_path + class_pictures[exemp];
      if (exemp % kFold == fold) {
        dataset.pictures_path.push_back(filename);
        dataset.labels.push_back(class_idx);
      }
    }
  }
  dataset.pictures.resize(dataset.labels.size());
  return dataset;
};
