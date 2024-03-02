#include "Mesh.hpp"

namespace potato {
    // Compute bounds for single face
    void computeBounds(vector<Vert> &vertices, Face &face, BoundBoxf &box, bool startBox) {
        
        // If restarting box, start to first value index
        if(startBox) {
            box.start = vertices.at(face.indices.at(0)).pos;
            box.end = vertices.at(face.indices.at(0)).pos;
        }
        
        // Loop through remaining indices, picking the smallest/largest         
        for(int i = 0; i < face.indices.size(); i++) {
            Vec3f v = vertices.at(face.indices.at(i)).pos;
            box.start = minV(box.start, v);
            box.end = maxV(box.end, v);
        }
    };

    // Compute bounds for ENTIRE mesh
    void computeBounds(PolyMesh *mesh, BoundBoxf &box, bool startBox) {
        
        // For each face...
        vector<Face> faces = mesh->getFaces();
        vector<Vert> vertices = mesh->getVertices();

        for(int i = 0; i < faces.size(); i++) {
            computeBounds(vertices, faces.at(i), box, startBox);
            startBox = false;
        }
    };
  /*
    inline Vec3<float> barycentric(BaryData &bd,                    
                            float x, float y) {
        Vec3<float> bary;
        bary.z = bd.AB.eval(x, y) / bd.Cval;
        bary.y = bd.AC.eval(x, y) / bd.Bval;
        bary.x = 1.0 - bary.y - bary.z;
        return bary;
    };
    
    inline void calculateTriangleBarycentric(  
                                        Vec3f &A,
                                        Vec3f &B,
                                        Vec3f &C,
                                        vector<Vec3f> &allBary) {

        BoundBox bb = computeBounds(A,B,C);
        int sy = floor(bb.start.y);
        int ey = floor(bb.end.y);
        int sx = floor(bb.start.x);
        int ex = floor(bb.end.x);
        BaryData bd(A,B,C);
        
        for(int y = sy; y <= ey; y++) {
            for(int x = sx; x <= ex; x++) {
                Vec3<float> bary = barycentric(bd,float(x),float(y));
                if(bary.x > 0 && bary.y > 0 && bary.z > 0) {
                    allBary.push_back(bary);
                }
            }
        }
        };*/
    /*
    void fillTriangle(  Image<Vec3f> *image,
                        Vec3f A, Vec3f B, Vec3f C) {
        BoundBox bb = computeBounds(A,B,C);
        for(auto y = bb.start.y; y <= bb.end.y; y += 1) {
            for(auto x = bb.start.x; x <= bb.end.x; x += 1) {
                Vec3<float> bary = barycentric(A,B,C,float(x),float(y));
                if(bary.x > 0 && bary.y > 0 && bary.z > 0) {
                    Vec3<S> color = Vec3<S>(
                        static_cast<S>(255.0*bary.x),
                        static_cast<S>(255.0*bary.y),
                        static_cast<S>(255.0*bary.z)
                    );
                    image->setPixel(static_cast<int>(x),
                                    static_cast<int>(y),
                                    color);
                }
            }
        }
    };*/
/*template<typename T>
    BoundBox<T> computeBounds(Vec3<T> A, Vec3<T> B, Vec3<T> C) {
        BoundBox<T> box;
        box.start = min(min(A,B),C);
        box.end = max(max(A,B),C);        
        return box;
==== BASE ====
};*/


};
