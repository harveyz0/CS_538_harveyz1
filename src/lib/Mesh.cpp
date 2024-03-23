#include "Mesh.hpp"
#include <cmath>
#include <stdexcept>

namespace potato {
    // Compute bounds for single face
    void computeBounds(vector<Vert> &vertices, Face &face, BoundBoxf &box,
                       bool startBox) {

        // If restarting box, start to first value index
        if (startBox) {
            box.start = vertices.at(face.indices.at(0)).pos;
            box.end   = vertices.at(face.indices.at(0)).pos;
        }

        // Loop through remaining indices, picking the smallest/largest
        for (int i = 0; i < face.indices.size(); i++) {
            Vec3f v   = vertices.at(face.indices.at(i)).pos;
            box.start = minV(box.start, v);
            box.end   = maxV(box.end, v);
        }
    };

    // Compute bounds for ENTIRE mesh
    void computeBounds(PolyMesh *mesh, BoundBoxf &box, bool startBox) {

        // For each face...
        vector<Face> faces    = mesh->getFaces();
        vector<Vert> vertices = mesh->getVertices();

        for (int i = 0; i < faces.size(); i++) {
            computeBounds(vertices, faces.at(i), box, startBox);
            startBox = false;
        }
    };

    vector<Face> Face::faceToTriangles(const vector<Vert> &verts) {
        if (this->isConcave(verts)) {
            return concaveFaceToTriangles(verts, *this);
        }
        vector<Face> indices;
        do{
            try{
                indices.push_back(faceToNextTriangle(verts));
    } catch(std::logic_error ex){
            cout << "Have polygon of " << this->indices.size() << " verticies left";
            break;
        }
        }while(true);
        //int i = 0;
        //for(i = poly.indices.size()-3; i >= 0; i -= 2){
        //    indices.push_back(Face(poly.indices.at(i), poly.indices.at(i+1), poly.indices.at(i+2)));
        //}
        //cout << "################# got to i " << i << endl;

        return indices;
    };

    Face Face::faceToNextTriangle(const vector<Vert> &verts) {
        if(this->indices.size() < 3){
            throw std::logic_error("Polygon is to small");
        }
        Face newFace(this->indices.at(0),this->indices.at(1),this->indices.at(2));
        this->indices.erase(std::next(this->indices.begin(), 1));
        return newFace;
    }

    vector<Face> concaveFaceToTriangles(const vector<Vert> &verts, const Face &poly){
            throw std::logic_error("NotImplemented");
    return vector<Face>();
};
}; // namespace potato
