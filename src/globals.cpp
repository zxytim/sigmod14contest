//File: globals.cpp
//Date: Sat Apr 05 16:05:20 2014 +0800
//Author: Yuxin Wu <ppwwyyxxc@gmail.com>

#include "globals.h"
#include "lib/common.h"

using namespace std;

// global variables
mutex tag_read_mt;
bool tag_read = false;
condition_variable tag_read_cv;

mutex friends_hash_built_mt;
bool friends_hash_built = false;
condition_variable friends_hash_built_cv;

Timer globaltimer;

ThreadPool* threadpool;

vector<double> tot_time(5, 0);

unordered_set<string, StringHashFunc> q4_tag_set;
// global variables
