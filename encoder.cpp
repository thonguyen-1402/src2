
#include <iostream>
#include<string.h>
#include<math.h>
#include<vector>
#include<fstream>
#include<iostream>
#include<sstream>
#include"octree.h"
using namespace std;

//entering : name of the prog - the name of the ply file - expected error
int main (int argc, char *argv[]) {
   
    point maxbounds{99999999.0f, 999999999.0f, 99999999.0f};  // Use floating-point literals
    point minbounds{-99999999.0f, -999999999.0f, -99999999.0f};
     float error = atof(argv[2]);
    string filename = argv[1];
    vector<pair <point, color>> data;
    data = read_pc_to_octree(filename);
    for ( int i = 0 ; i < data.size(); i++) {
        if (maxbounds.x < data[i].first.x) maxbounds.x = data[i].first.x;
        if (maxbounds.y < data[i].first.x) maxbounds.y = data[i].first.y;
        if (maxbounds.z < data[i].first.z) maxbounds.z = data[i].first.z;
        if (minbounds.x > data[i].first.x) minbounds.x = data[i].first.x;
        if (minbounds.y > data[i].first.y) minbounds.y = data[i].first.y;
        if (minbounds.z > data[i].first.z) minbounds.z = data[i].first.z;
    }
    octree root(maxbounds, minbounds);
   root.turn_into_octree(&root,data, error);
   const char*s = argv[3];
   string str(s);
   root. octree_into_bin(str);


}

