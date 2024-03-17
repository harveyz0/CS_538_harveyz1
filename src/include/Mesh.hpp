#pragma once

#include <stdexcept>
#define _USE_MATH_DEFINES
#include "BoundBox.hpp"
#include "Object3D.hpp"
#include "Vector.hpp"
#include <cmath>
#include <iostream>
#include <vector>
using namespace std;

namespace potato {

    struct Vert {
        Vec3f pos{};
        Vec4f color{}; // [0,1]

        Vert  operator+(const Vert &other) const {
            return {pos + other.pos, color + other.color};
        };

        Vert operator-(const Vert &other) const {
            return {pos - other.pos, color - other.color};
        };

        Vert operator*(const Vert &other) const {
            return {pos * other.pos, color * other.color};
        };

        Vert operator*(const float &w) const { return {pos * w, color * w}; };
    };

    struct Face {
        vector<unsigned int> indices{};
        Face(unsigned int A, unsigned int B, unsigned int C) {
            this->indices.push_back(A);
            this->indices.push_back(B);
            this->indices.push_back(C);
        }
        Face() = default;

        bool isConcave(const vector<Vert> &verts) const {
            if (3 > this->indices.size()) {
                throw std::out_of_range(
                    "Can not run isConcave on face with less than 3 indices");
            }
            //int  start = this->indices.size() - 2;
            int  start = 0;
            int  next = 1;
            bool isLessThanZero =
                0 > verts.at(start).pos.cross(verts.at(next).pos).z;
            //cout << "isLessThanZero " << isLessThanZero << " verts.at(start + 1).pos.cross(verts.at(start).pos).z " << verts.at(start + 1).pos << " pos " << verts.at(start).pos << endl;
            cout << "#### isLessThanZero " << isLessThanZero << " verts.at(start) " << verts.at(start).pos << " verts.at(next) ";
            cout << verts.at(next).pos << " z " << verts.at(start).pos.cross(verts.at(next).pos).z << endl;
            for (start = start + 1, next = next + 1; start < this->indices.size(); ++start, ++next) {
            if(next == this->indices.size()){
                next = 0;
            }
            auto z = verts.at(start).pos.cross(verts.at(next).pos).z;
            cout << "Loop isLessThanZero " << isLessThanZero << " verts.at(start) " << verts.at(start).pos << " verts.at(next) ";
            cout << verts.at(next).pos << " z " << z << endl;
                if ((0 > z &&
                     !isLessThanZero) ||
                    (0 < z &&
                     isLessThanZero)) {
                    return true;
                }
            }
            return false;
        };
        bool isTriangle() { return indices.size() == 3; };
        Face dropIndex(unsigned int index) {
            Face newFace(*this);
            newFace.indices.erase(std::next(newFace.indices.begin(), index));
            return newFace;
        };
        vector<Face> faceToTriangles(const vector<Vert> &verts);
        Face faceToNextTriangle(const vector<Vert> &verts);
    };

    class PolyMesh : public Object3D {
      protected:
        vector<Vert> vertices{};
        vector<Face> faces{};
        unsigned int id{};

      public:
        PolyMesh() : Object3D(){};
        PolyMesh(const vector<Vert> &verts, const vector<Face> &face)
            : Object3D(), vertices(std::move(verts)), faces(std::move(face)){};
        virtual ~PolyMesh(){};

        vector<Vert> &getVertices() { return vertices; };
        vector<Face> &getFaces() { return faces; };
        void setId(unsigned int newId){ this->id = newId; }
    };

    // Compute bounds for single face
    void computeBounds(vector<Vert> &vertices, Face &face, BoundBoxf &box,
                       bool startBox = true);

    // Compute bounds for ENTIRE mesh
    void computeBounds(PolyMesh *mesh, BoundBoxf &box, bool startBox = true);

    vector<Face> faceToTriangles(const vector<Vert> &verts, const Face &poly);
    vector<Face> concaveFaceToTriangles(const vector<Vert> &verts,
                                        const Face         &poly);

}; // namespace potato
