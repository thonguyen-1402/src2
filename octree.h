#pragma once
#ifndef OCTREE
#define OCTREE

#include<iostream>
#include<string.h>
#include<math.h>
#include<vector>
#include<fstream>
#include<iostream>
#include<sstream>
using namespace std;
struct point {
    float x, y, z;
    int octant;
    friend point operator +(const point &p1, const point &p2) {
        point result;
        result.x = p1.x + p2.x;
        result.y = p1.y + p2.y;
        result.z = p1.z + p2.z;
        return result;
    }
    friend point operator *(const point &p1, const float scalar) {
        point result;
        result.x = p1.x*scalar;
        result.y = p1.y*scalar;
        result.z = p1.z*scalar;
        return result;
    }
    friend bool operator < (const point &p1, const point &p2) {
        return (p1.x < p2.x && p1.y < p2.y && p1.z < p2.z);
    }
    friend bool operator > (const point &p1, const point &p2) {
        return (p1.x > p2.x && p1.y > p2.y && p1.z > p2.z);
    }
    point& operator = (const point &p) {
        x = p.x;
        y = p.y;
        z = p.z;
        return *this;
    }

};
struct color {
    unsigned char R, B, G;
};


class octree {
protected:
color colors;
point maxbounds;
point minbounds;
octree*children[8];
public:
octree () {};
octree (point maxbounds_, point minbounds_) : maxbounds{maxbounds_}, minbounds{minbounds_} {
    for ( int i = 0 ; i < 8; i++) {
        children[i] = nullptr;
    }
}
  ~octree() {
        // Deallocate point array if it was allocated
        delete[] children;
    }


int get_octant (point p, point centre) {
        if (p.x > centre.x) p.octant |= 1;
        if (p.y > centre.y) p.octant |= 2;
        if (p.z > centre.z) p.octant |= 4;
        return p.octant;
}
float getdistance ( point p1, point p2){ return pow((p1.x -p2.x),2) + pow((p1.y -p2.y),2)+ pow((p1.z -p2.z),2); }
void build (octree*node, point point_,color colors_,float error) {
    float errorsq = error*error;
   if (!node) {
    cout << "unable to initialize a tree";
    return;
   }
   point centre = (maxbounds+minbounds)*0.5f;
   if (getdistance(point_, centre) <= errorsq) {
        node->colors = colors_;
        return;
   }
   int octant_ = get_octant(point_, centre); //get the location of the point in terms of octant
   if ( node->children[octant_] != nullptr ) {
    point new_maxbounds, new_minbounds;
       if (octant_ & 1)    new_maxbounds.x = centre.x; new_minbounds.x = minbounds.x; //determine the position of the octant to get the newbounds, which later used to make a new node
       if (octant_ & 2)    new_maxbounds.y = centre.y; new_minbounds.y = minbounds.y;
       if (octant_ & 4)    new_maxbounds.x = centre.z; new_minbounds.z = minbounds.z;     
       node->children[octant_] = new octree (new_maxbounds, new_minbounds);
}
    build ( node->children[octant_], point_,colors_, error);
}

void turn_into_octree(octree* node, vector<pair<point, color>>& pointdata, float error) {
    for (int i = 0 ; i < pointdata.size();i++) {
        build(node, pointdata[i].first, pointdata[i].second, error);
    }
}

void octree_into_bin(const string& filename) {
        ofstream outfile(filename, ios::binary);
        if (!outfile.is_open()) {
            cerr << "Error opening file for writing: " << filename << endl;
            return;
        }
        write_octree_to_bin(outfile);
        outfile.close();
        cout << "Octree has been written successfully to " << filename << endl;
    }

private:
    void write_octree_to_bin(ofstream& outfile) {
        outfile.write(reinterpret_cast<const char*>(&maxbounds), sizeof(maxbounds));
        outfile.write(reinterpret_cast<const char*>(&minbounds), sizeof(minbounds));
        outfile.write(reinterpret_cast<const char*>(&colors), sizeof(colors));
        for (int i = 0; i < 8; ++i) {
            bool haschild = (children[i] != nullptr);
            outfile.write(reinterpret_cast<const char*>(&haschild), sizeof(haschild));
            if (haschild) {
                children[i]->write_octree_to_bin(outfile);
            }
        }
    }

};
vector<pair <point, color>> read_pc_to_octree (const string& filename) {  //function that return a pair of vector
    ifstream file(filename); //khai bao luong thuoc doi tuong ifstream (dung de doc file)
    vector <pair <point, color>> pointdata;
    if(file.fail()) {
        cerr << "Error";
        exit(1);
    }
    string line;
    bool end_of_header = false;
    while(getline(file, line))  {
        if (!end_of_header) {
            if(line == "end_header") {
                end_of_header = true;
            }
            continue;
        }
        //now we process the data part
        stringstream ss(line);
        point p;
        color c;
        ss >> p.x >> p.y >> p.z >> c.R >> c.G >> c.B;
        pointdata.push_back(make_pair(p,c));
    }
    file.close();
    return pointdata;
    }
#endif
