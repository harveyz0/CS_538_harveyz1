#include "Mesh.hpp"
#include "generator.hpp"

namespace potato {
    PolyMesh *generateConvexPolygon(const Vec3f &center, float radius,
                                    int triangleCnt) {
        // Generate vertices
        PolyMesh *m                = new PolyMesh();
        double    angleInc         = double(2.0f * M_PI / (triangleCnt * 2.0f));
        float     colorAngleOffset = float(2.0f * M_PI / 3.0f);

        Vert      centerVert;
        centerVert.pos   = center;
        centerVert.color = Vec4f(1.0f, 1.0f, 1.0f, 1.0f);

        int  vcnt        = triangleCnt * 2;

        Face concave;

        for (int i = 0; i < vcnt; i++) {
            float angle = float(angleInc * i);
            Vert  v;
            v.pos =
                Vec3f(radius * cos(angle), radius * sin(angle), 0.0f) + center;
            v.pos.x = std::round(v.pos.x);
            v.pos.y = std::round(v.pos.y);
            float r = std::max(0.0f, cos(angle));
            float g = std::max(0.0f, cos(angle - colorAngleOffset));
            float b = std::max(0.0f, cos(angle - 2.0f * colorAngleOffset));
            v.color = Vec4f(r, g, b, 1.0f);
            m->getVertices().push_back(v);
            concave.indices.push_back(m->getVertices().size() - 1);
        }
        m->getFaces().push_back(concave);

        return m;
    };

    PolyMesh *generateConcavePolygon(const Vec3f &center, float radius,
                                     int triangleCnt) {
        // Generate vertices
        PolyMesh *m                = new PolyMesh();
        double    angleInc         = double(2.0f * M_PI / (triangleCnt * 2.0f));
        float     colorAngleOffset = float(2.0f * M_PI / 3.0f);

        Vert      centerVert;
        centerVert.pos   = center;
        centerVert.color = Vec4f(1.0f, 1.0f, 1.0f, 1.0f);

        int vcnt         = triangleCnt * 2;
        // int  vcnt        = triangleCnt ;

        Face concave;

        for (int i = 0; i < vcnt; i++) {
            float angle = float(angleInc * i);
            Vert  v;
            v.pos =
                Vec3f(radius * cos(angle), radius * sin(angle), 0.0f) + center;
            v.pos.x = std::round(v.pos.x);
            v.pos.y = std::round(v.pos.y);
            float r = std::max(0.0f, cos(angle));
            float g = std::max(0.0f, cos(angle - colorAngleOffset));
            float b = std::max(0.0f, cos(angle - 2.0f * colorAngleOffset));
            v.color = Vec4f(r, g, b, 1.0f);
            m->getVertices().push_back(v);
            concave.indices.push_back(m->getVertices().size() - 1);
        }
        m->getVertices().push_back(centerVert);
        // The insert is so we can get the concave to not align with a scan line
        concave.indices.insert(
            std::next(concave.indices.begin(), std::round(vcnt / 4)),
            m->getVertices().size() - 1);

        m->getFaces().push_back(concave);
        return m;
    };

    void shortenVerticies(PolyMesh &toCheck) {

        unsigned int point1 = 0;
        unsigned int point2 = 1;
        unsigned int point3 = 2;

        do {

        } while(point1 < 

        for(int i = toCheck.getVertices().size(); i > 0; --i){
            

        }
    };

    PolyMesh shrinkLines(const Face &face, const vector<Vert> &verts) {
        if (face.indices.size() < 3) {
            throw std::logic_error("Polygon is to small");
        }

        unsigned int         point1 = 0;
        unsigned int         point2 = 1;
        unsigned int         point3 = 2;

        vector<unsigned int> indices{};
        vector<Vert>         newVerts{};

        PolyMesh             newMesh{};

        do {
            const Vec3f &pos1 = verts.at(face.indices.at(point1)).pos;
            const Vec3f &pos2 = verts.at(face.indices.at(point2)).pos;
            const Vec3f &pos3 = verts.at(face.indices.at(point3)).pos;

            // if (pos1.y < pos2.y && pos2.y > pos3.y) {
            //  shrink pos2
            //}
            // else
            if (pos1.y > pos2.y && pos3.y > pos2.y) {
                indices.push_back(
                    newMesh.pushVert(verts.at(face.indices.at(point1))));
                unsigned int index = newMesh.pushVert(verts.at(face.indices.at(point2)));
                indices.push_back(index);
                indices.push_back(index);
                indices.push_back(
                    newMesh.pushVert(verts.at(face.indices.at(point3))));
            } else {
                indices.push_back(
                    newMesh.pushVert(verts.at(face.indices.at(point1))));
                indices.push_back(
                    newMesh.pushVert(verts.at(face.indices.at(point2))));
                indices.push_back(
                    newMesh.pushVert(verts.at(face.indices.at(point3))));
            }

            ++point1;
            ++point2;
            ++point3;
            if (point2 == face.indices.size()) {
                point2 = 0;
            }
            if (point3 == face.indices.size()) {
                point3 = 0;
            }
        } while (point1 < face.indices.size());
    };

    PolyMesh *generateTestFan(const Vec3f &center, float radius,
                              int triangleCnt) {
        // Generate vertices
        PolyMesh *m                = new PolyMesh();
        double    angleInc         = double(2.0f * M_PI / (triangleCnt * 2.0f));
        float     colorAngleOffset = float(2.0f * M_PI / 3.0f);

        Vert      centerVert;
        centerVert.pos   = center;
        centerVert.color = Vec4f(1.0f, 1.0f, 1.0f, 1.0f);
        m->getVertices().push_back(centerVert);

        int vcnt = triangleCnt * 2;

        for (int i = 0; i < vcnt; i++) {
            float angle = float(angleInc * i);
            Vert  v;
            v.pos =
                Vec3f(radius * cos(angle), radius * sin(angle), 0.0f) + center;
            v.pos.x = std::round(v.pos.x);
            v.pos.y = std::round(v.pos.y);
            float r = std::max(0.0f, cos(angle));
            float g = std::max(0.0f, cos(angle - colorAngleOffset));
            float b = std::max(0.0f, cos(angle - 2.0f * colorAngleOffset));
            v.color = Vec4f(r, g, b, 1.0f);
            m->getVertices().push_back(v);
        }

        // Generate triangles
        for (int i = 0; i < vcnt; i += 2) {
            Face f;
            f.indices.push_back(0);
            f.indices.push_back(i + 1);
            f.indices.push_back(i + 2);
            m->getFaces().push_back(f);
        }

        return m;
    };
}; // namespace potato
