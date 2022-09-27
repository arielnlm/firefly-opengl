#ifndef PLANE_H
#define PLANE_H

#include <utils/chunk.h>

#include <vector>

using namespace std;

class Plane {
public:
    vector<vector<Chunk>> chunks;

    Plane(vector<vector<Chunk>> chunks)
    {
        this->chunks = chunks;
    }

    Plane()
    {

    }

    // render the mesh
    void Draw(glm::vec3 CameraPos) 
    {
        for (int i = 0 ; i < chunks.size() ; i++){
            for (int j = 0 ; j < chunks[i].size() ; j++){
                chunks[i][j].Draw(CameraPos);
            }
        }
    }

    void addChunks(vector<vector<Chunk>> chunks)
    {
        this->chunks = chunks;
    }
};
#endif
