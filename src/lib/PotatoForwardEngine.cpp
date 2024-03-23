#include "PotatoForwardEngine.hpp"
#include "Mesh.hpp"
#include "Settings.hpp"
#include <stdexcept>

PotatoForwardEngine::PotatoForwardEngine(int windowWidth, int windowHeight) : PotatoRenderEngine(windowWidth, windowHeight) {
    // For now, generate simple fan
    PolyMesh *m = generateConvexPolygon(Vec3f(windowWidth/2.0f, windowHeight/2.0f, 0.0f),
                                    windowHeight/3.0f, GEO_FAN_BLADE_CNT);
    allMeshes.push_back(m);
    //return;
    
    vector<Face> faces = m->getFaces().at(0).faceToTriangles(m->getVertices());
    //vector<Face> faces;

    PolyMesh *split = new PolyMesh(m->getVertices(), faces);
    split->setId(4096);
    allMeshes.push_back(split);
    //for (auto f : split->getFaces()) {
    //    cout << " FACE " << endl;
    //    for(auto i : f.indices){
    //        cout << "indices i " << i << " vert pos " << split->getVertices().at(i).pos << endl;
    //    }
    //}
}

PotatoForwardEngine::~PotatoForwardEngine() { 
    // Clean up meshes
    for(int i = 0; i < allMeshes.size(); i++) {
        delete allMeshes.at(i);
    }
    allMeshes.clear();

    allFragments.clear();
}

void PotatoForwardEngine::mergeFragments(vector<Fragment> &fragList, Image<Vec3f> *drawBuffer) {
    // For now, just blindly write all fragments to buffer
    for(int i = 0; i < fragList.size(); i++) {
        Fragment f = fragList.at(i);
        try{
        drawBuffer->setPixel(f.pos.x, f.pos.y, Vec3f(f.color));
        }catch(const std::out_of_range& ex){
            cout << "ERROR : " << ex.what() << endl;
        }
    }
}

void PotatoForwardEngine::renderToDrawBuffer(Image<Vec3f> *drawBuffer) {
    
    // Clear list of fragments
    allFragments.clear();

    for(int i = 0; i < allMeshes.size(); i++) {
        PolyMesh *mesh = allMeshes.at(i);

        // Get fragments for inside of polygons
        if(!SKIP_FILL){
        fillTriangles(mesh, allFragments);
        }

        

        // Get fragments for lines
        drawLines(mesh, allFragments, DRAW_LINES_AS_WIREFRAME);
    }

    // Merge fragments
    mergeFragments(allFragments, drawBuffer);
}
