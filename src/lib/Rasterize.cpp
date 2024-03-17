#include "Bary.hpp"
#include "BoundBox.hpp"
#include "Fragment.hpp"
#include "Line.hpp"
#include "Mesh.hpp"
#include "Rasterize.hpp"
#include "Vector.hpp"
#include <algorithm>
#include <stdexcept>

namespace potato {
    // Generates mesh for testing
    PolyMesh *generateTestFan(const Vec3f &center, float radius,
                              int triangleCnt) {
        // Generate vertices
        PolyMesh *m                = new PolyMesh();
        double    angleInc         = double(2.0f * M_PI / (triangleCnt * 2.0f));
        float     colorAngleOffset = float(2.0f * M_PI / 3.0f);

        Vert      centerVert;
        centerVert.pos   = center;
        centerVert.color = Vec4f(1.0f, 1.0f, 1.0f, 1.0f);
        //m->getVertices().push_back(centerVert);

        int  vcnt        = triangleCnt * 2;
        //int  vcnt        = triangleCnt ;

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
            cout << "Pushing on " << v.pos << endl;
        }


        m->getFaces().push_back(concave);

        // Generate triangles
        // for (int i = 0; i < vcnt; i += 2) {
        //    Face f;
        //    f.indices.push_back(0);
        //    f.indices.push_back(i + 1);
        //    f.indices.push_back(i + 2);
        //    m->getFaces().push_back(f);
        //}

        return m;
    };

    // Draws lines for mesh
    void drawLines(PolyMesh *mesh, vector<Fragment> &fragList, bool wireframe) {
        vector<Vert> vertices = mesh->getVertices();
        vector<Face> faces    = mesh->getFaces();

        void (*drawLine)(Vert &, Vert &, vector<Fragment> &, bool){};
        if (LINE_ALGORITHM == LINE_DDA) {
            drawLine = &drawLineDDA;
        } else if (LINE_ALGORITHM == LINE_MID) {
            drawLine = &drawLineMid;
        } else if (LINE_ALGORITHM == NO_LINE) {
            drawLine = &drawPoints;
        } else {
            throw std::invalid_argument("Bad line drawing algorithm!");
        }

        for (int i = 0; i < faces.size(); i++) {
            // For each line...
            for (int k = 0; k < faces.at(i).indices.size(); k++) {
                int firstIndex  = faces.at(i).indices.at(k);
                int secondIndex = faces.at(i).indices.at(
                    (k + 1) % faces.at(i).indices.size());
                drawLine(vertices.at(firstIndex), vertices.at(secondIndex),
                         fragList, wireframe);

                //drawPoints(vertices.at(firstIndex), vertices.at(secondIndex),
                //         fragList, true);
                // cout << "Face at " << i << " is " <<
                // faces.at(i).isConcave(mesh->getVertices()) << endl;
            }
        }
    };

    // Fills triangles for mesh
    void fillTriangles(PolyMesh *mesh, vector<Fragment> &fragList) {
        vector<Vert> vertices = mesh->getVertices();
        vector<Face> faces    = mesh->getFaces();
        for (int i = 0; i < faces.size(); i++) {
            fillTriangle(vertices, faces.at(i), fragList);
        }
    };

    // Computes interpolated fragment
    Fragment computeFragment(Vert &vA, Vert &vB, Vert &vC, Vec3f &bc) {
        Vert     FV = (vA * bc.x) + (vB * bc.y) + (vC * bc.z);
        Fragment frag;
        frag.pos   = roundV(FV.pos);
        frag.color = FV.color;
        return frag;
    };

    void drawLineDDA(Vert &startVert, Vert &endVert, vector<Fragment> &fragList,
                     bool wireframe) {
        auto  dx    = endVert.pos.x - startVert.pos.x;
        auto  dy    = endVert.pos.y - startVert.pos.y;
        int   steps = 0;
        float xInc, yInc;
        float x = startVert.pos.x;
        float y = startVert.pos.y;

        if (abs(dx) > abs(dy)) {
            steps = abs(dx);
        } else {
            steps = abs(dy);
        }

        Vec4f colorInc = (endVert.color - startVert.color) / steps;
        Vec4f curColor(startVert.color + colorInc);
        if (wireframe) {
            curColor = Vec4f(1.0, 1.0, 1.0, 1.0);
        }

        xInc = float(dx) / float(steps);
        yInc = float(dy) / float(steps);

        fragList.push_back(Fragment((int)round(x), (int)round(y), curColor));

        for (int k = 0; k < steps; k++) {
            x += xInc;
            y += yInc;
            if (!wireframe) {
                curColor = curColor + colorInc;
            }
            fragList.push_back(
                Fragment((int)round(x), (int)round(y), curColor));
        }
    };

    void drawLineMid(Vert &startVert, Vert &endVert, vector<Fragment> &fragList,
                     bool wireframe) {

        // When you need to flip x and y do all xs and ys flip?
        Vec3<float> newStart(startVert.pos);
        Vec3<float> newEnd(endVert.pos);

        int         dx   = newEnd.x - newStart.x;
        int         dy   = newEnd.y - newStart.y;
        bool        swap = abs(dx) < abs(dy);
        if (swap) {
            std::swap(dx, dy);
            std::swap(newStart.x, newStart.y);
            std::swap(newEnd.x, newEnd.y);
        }
        Vec3f startColor = startVert.color;
        Vec3f endColor   = endVert.color;
        if (dx < 0) {
            std::swap(newStart, newEnd);
            std::swap(startColor, endColor);
            dx = -dx;
            dy = -dy;
        }

        float change = 1;
        float y = newStart.y; // I don't really understand why this doesn't get
                              // changed in the dy < 0
        if (dy < 0) {
            change = -1;
            dy     = -dy;
            std::swap(newStart.y,
                      newEnd.y); // But we leave our y as the start position
        }
        float x        = newStart.x;
        float stop     = newEnd.x;

        Vec3f color    = startColor;
        Vec3f colorInc = (endColor - startColor) / (stop - x);
        if (wireframe) {
            color = Vec3f(1.0, 1.0, 1.0);
        }
        float d = calculateMidpoint(newStart.x + 1, newStart.y + 0.5, newStart,
                                    newEnd);
        for (; x <= stop; ++x) {
            if (swap) {
                fragList.push_back(Fragment(y, x, color));
            } else {
                fragList.push_back(Fragment(x, y, color));
            }
            if (!wireframe) {
                color = color + colorInc;
            }
            if (d < 0) {
                y = y + change;
                d = d + (dx - dy);
            } else {
                d = d - dy;
            }
        }
    };

    void drawPoints(Vert &startVert, Vert &endVert, vector<Fragment> &fragList,
                    bool wireframe) {
        Vec4f startColor(startVert.color);
        Vec4f endColor(endVert.color);
        if (wireframe) {
            startColor = Vec4f(1.0f, 1.0f, 1.0f, 1.0f);
            endColor   = Vec4f(1.0f, 1.0f, 1.0f, 1.0f);
        }
        fragList.push_back(
            Fragment(startVert.pos.x, startVert.pos.y, startColor));
        fragList.push_back(Fragment(endVert.pos.x, endVert.pos.y, endColor));
    };

    void fillTriangle(vector<Vert> &vertices, Face &face,
                      vector<Fragment> &fragList) {
        BoundBox<float> fbox;
        computeBounds(vertices, face, fbox, true);
        BoundBox<int> ibox = convertBoundBox(fbox);
        Vert          vA   = vertices.at(face.indices.at(0));
        Vert          vB   = vertices.at(face.indices.at(1));
        Vert          vC   = vertices.at(face.indices.at(2));

        BaryData      bd(vA, vB, vC);

        for (float y = static_cast<float>(ibox.start.y);
             y <= static_cast<float>(ibox.end.y); y += 1.0f) {
            for (float x = static_cast<float>(ibox.start.x);
                x <= static_cast<float>(ibox.end.x); x += 1.0f) {
                // Vec3<float> bary = barycentric(A,B,C,float(x),float(y));
                Vec3<float> bary = barycentric(bd, x, y);
                if (bary.x > 0 && bary.y > 0 && bary.z > 0) {
                    Vec3<float> bc   = Vec3<float>(bary.x, bary.y, bary.z);
                    Fragment    comp = computeFragment(vA, vB, vC, bc);

                    fragList.push_back(comp);
                }
            }
        }
    };
}; // namespace potato
