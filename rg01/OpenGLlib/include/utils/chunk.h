#ifndef CHUNK_H
#define CHUNK_H

#include <glad/glad.h> // holds all OpenGL type declarations

#include <glm/glm.hpp>

#include <utils/shader.h>
#include <utils/mesh.h>

using namespace std;

class Chunk {
public:
    /*  Mesh Data  */
    Mesh lvl1;
    Mesh lvl2;
    Mesh lvl3;

    glm::vec3 p1;
    glm::vec3 p2;
    glm::vec3 p3;
    glm::vec3 p4;

    float CLOSE_LVL_1 = 35;
    float CLOSE_LVL_2 = 65;
    float CLOSE_LVL_3 = 95;

    /*  Functions  */
    // constructor
    Chunk(Mesh lvl1, Mesh lvl2, Mesh lvl3, glm::vec3 p1, glm::vec3 p2, glm::vec3 p3, glm::vec3 p4)
    {
        this->lvl1 = lvl1;
        this->lvl2 = lvl2;
        this->lvl3 = lvl3;

        this->p1 = p1;
        this->p2 = p2;
        this->p3 = p3;
        this->p4 = p4;
    }

    Chunk()
    {

    }

    void Draw(glm::vec3 CameraPos) 
    {     
        float minDist = findClosest(CameraPos);
        if (minDist < CLOSE_LVL_1)
            lvl1.Draw();
        else if (minDist < CLOSE_LVL_2)
            lvl2.Draw();
        else if (minDist < CLOSE_LVL_3)
            lvl3.Draw();
    }

    void initMashes(Mesh lvl1, Mesh lvl2, Mesh lvl3)
    {
        this->lvl1 = lvl1;
        this->lvl2 = lvl2;
        this->lvl3 = lvl3;
    }

    void initPoints(glm::vec3 p1, glm::vec3 p2, glm::vec3 p3, glm::vec3 p4)
    {
        this->p1 = p1;
        this->p2 = p2;
        this->p3 = p3;
        this->p4 = p4;
    }

    void setLvl1(Mesh lvl1)
    {
        this->lvl1 = lvl1;
    }

    void setLvl2(Mesh lvl2)
    {
        this->lvl2 = lvl2;
    }

    void setLvl3(Mesh lvl3)
    {
        this->lvl3 = lvl3;
    }

    void setCloseLevels(float lvl1, float lvl2, float lvl3)
    {
        this->CLOSE_LVL_1 = lvl1;
        this->CLOSE_LVL_2 = lvl2;
        this->CLOSE_LVL_3 = lvl3;
    }

private: 
    float findClosest(glm::vec3 CameraPos)
    {
        float min = glm::distance(CameraPos,p1);
        
        if (min > glm::distance(CameraPos,p2))
            min = glm::distance(CameraPos,p2);
        if (min > glm::distance(CameraPos,p3))
            min = glm::distance(CameraPos,p3);
        if (min > glm::distance(CameraPos,p4))
            min = glm::distance(CameraPos,p4);
        return min;
        
        /*
        float br;
        for (int i = 0; i < lvl1.vertices.size() ; ++i){
            glm::vec3 tmp(lvl1.vertices[i].Position);
            br = glm::distance(CameraPos,tmp);
            if (min > br)
                min = br;
        }
        for (int i = 0; i < lvl2.vertices.size() ; ++i){
            glm::vec3 tmp(lvl2.vertices[i].Position);
            br = glm::distance(CameraPos,tmp);
            if (min > br)
                min = br;
        }
        for (int i = 0; i < lvl3.vertices.size() ; ++i){
            glm::vec3 tmp(lvl3.vertices[i].Position);
            br = glm::distance(CameraPos,tmp);
            if (min > br)
                min = br;
        }
        return min;
        */
    }
};
#endif