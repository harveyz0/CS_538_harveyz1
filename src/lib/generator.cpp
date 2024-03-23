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
        concave.indices.push_back(m->getVertices().size() - 1);

        m->getFaces().push_back(concave);
        return m;
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
