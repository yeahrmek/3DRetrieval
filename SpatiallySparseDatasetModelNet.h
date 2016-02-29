#pragma once
#include "SpatiallySparseDataset.h"
#include <iostream>

SpatiallySparseDataset ModelNetTrainSet(int renderSize, int kFold, int fold);
SpatiallySparseDataset ModelNetTestSet(int renderSize, int kFold, int fold);
