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

    class Vert {
      public:
        Vec3f pos{};
        Vec4f color{}; // [0,1]

        Vert()                = default;
        // Vert(Vert const &old) : pos(old.pos), color(old.color){}
        Vert(Vert const &old) = default;

        Vert(Vec3f newPos, Vec4f newColor) : pos(newPos), color(newColor) {}
        Vert operator+(const Vert &other) const {
            return {pos + other.pos, color + other.color};
        };

        Vert operator-(const Vert &other) const {
            return {pos - other.pos, color - other.color};
        };

        Vert operator*(const Vert &other) const {
            return {pos * other.pos, color * other.color};
        };

        Vert operator*(const float &w) const { return {pos * w, color * w}; };
        ostream &operator<<(ostream &os) const {
            return os << "(" << this->pos.x << "," << this->pos.y << ","
                      << this->pos.z << ")";
        };
    };

    struct Face {
        vector<unsigned int> indices{};
        Face(unsigned int A, unsigned int B, unsigned int C) {
            this->indices.push_back(A);
            this->indices.push_back(B);
            this->indices.push_back(C);
        }
        Face() = default;
        Face(vector<unsigned int> newIndices) : indices(newIndices) {};

        template <typename T>
        inline Vec3<T> crossProductLines(const Vec3<T> &point1,
                                         const Vec3<T> &point2,
                                         const Vec3<T> &point3) const {
            return (point2 - point1).cross(point3 - point2);
        };

        bool isConcave(const vector<Vert> &verts) const {
            if (3 > this->indices.size()) {
                throw std::out_of_range(
                    "Can not run isConcave on face with less than 3 indices");
            }
            unsigned int point1 = 0;
            unsigned int point2 = 1;
            unsigned int point3 = 2;
            bool         isLessThanZero =
                0 > this->crossProductLines(verts.at(point1).pos,
                                            verts.at(point2).pos,
                                            verts.at(point3).pos)
                        .z;
            ++point1;
            ++point2;
            ++point3;
            while (point1 < this->indices.size()) {
                auto z = this->crossProductLines(
                                 verts.at(this->indices.at(point1)).pos,
                                 verts.at(this->indices.at(point2)).pos,
                                 verts.at(this->indices.at(point3)).pos)
                             .z;
                if ((0 > z && !isLessThanZero) || (0 < z && isLessThanZero)) {
                    return true;
                }
                ++point1;
                ++point2;
                ++point3;
                if (point2 == this->indices.size()) {
                    point2 = 0;
                }
                if (point3 == this->indices.size()) {
                    point3 = 0;
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
        Face         faceToNextTriangle(const vector<Vert> &verts);
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
        void          setId(unsigned int newId) { this->id = newId; };

        unsigned int  pushVert(const Vert &newVert) {
            this->vertices.push_back(newVert);
            return this->vertices.size() - 1;
        };

        void pushFace(Face &face) { this->faces.push_back(face); };
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
