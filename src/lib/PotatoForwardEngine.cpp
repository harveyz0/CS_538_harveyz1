#include "PotatoForwardEngine.hpp"
#include "Clipping.hpp"
#include "Mesh.hpp"
#include "Settings.hpp"
#include <iterator>
#include <stdexcept>

PotatoForwardEngine::PotatoForwardEngine(int windowWidth, int windowHeight)
    : PotatoRenderEngine(windowWidth, windowHeight) {
    // For now, generate simple fan
    // PolyMesh *m = generateConvexPolygon(Vec3f(windowWidth/2.0f,
    // windowHeight/2.0f, 0.0f),
    //                                windowHeight/3.0f, GEO_FAN_BLADE_CNT);
    // allMeshes.push_back(m);
    // return;

    // vector<Face> faces =
    // m->getFaces().at(0).faceToTriangles(m->getVertices()); vector<Face>
    // faces;

    // PolyMesh *split = new PolyMesh(m->getVertices(), faces);
    // split->setId(4096);
    // allMeshes.push_back(split);
    // for (auto f : split->getFaces()) {
    //     cout << " FACE " << endl;
    //     for(auto i : f.indices){
    //         cout << "indices i " << i << " vert pos " <<
    //         split->getVertices().at(i).pos << endl;
    //     }
    // }
    //

    PolyMesh *meshes = loadOBJModel("./sampleModels/teapot.obj");
    allMeshes.push_back(meshes);
    renderMeshes.push_back(new PolyMesh(*meshes));
}

PotatoForwardEngine::~PotatoForwardEngine() {
    // Clean up meshes
    for (int i = 0; i < allMeshes.size(); i++) {
        delete allMeshes.at(i);
    }
    allMeshes.clear();
    for (int i = 0; i < renderMeshes.size(); i++) {
        delete renderMeshes.at(i);
    }
    renderMeshes.clear();

    allFragments.clear();
}

void PotatoForwardEngine::mergeFragments(vector<Fragment> &fragList,
                                         Image<Vec3f>     *drawBuffer) {
    // For now, just blindly write all fragments to buffer
    for (int i = 0; i < fragList.size(); i++) {
        Fragment f = fragList.at(i);
        try {
            drawBuffer->setPixel(f.pos.x, f.pos.y, Vec3f(f.color));
        } catch (const std::out_of_range &ex) {
            cout << "ERROR : " << ex.what() << endl;
        }
    }
}

void PotatoForwardEngine::renderToDrawBuffer(Image<Vec3f> *drawBuffer) {

    // Clear list of fragments
    allFragments.clear();


    for (int i = 0; i < allMeshes.size(); i++) {
        PolyMesh *inputMesh = allMeshes.at(i);
        PolyMesh *mesh = renderMeshes.at(i);

        Mat4f m(1.0);
        processGeometryOneMesh(inputMesh, m, m, m, mesh);

        // Get fragments for inside of polygons
        if (!SKIP_FILL) {
            fillTriangles(mesh, allFragments);
        }

        // Get fragments for lines
        drawLines(mesh, allFragments, DRAW_LINES_AS_WIREFRAME);
    }

    // Merge fragments
    mergeFragments(allFragments, drawBuffer);
}

void PotatoForwardEngine::processGeometryOneMesh(PolyMesh *inputMesh,
                                                 Mat4f    &modelMat,
                                                 Mat4f &viewMat, Mat4f &projMat,
                                                 PolyMesh *outMesh) {
    vector<int> clips;
    for (int i = 0; i < inputMesh->getVertices().size(); ++i) {

        Vec4f pos(inputMesh->getVertices().at(i).pos, 1.0f);

        Vec4f newPos = projMat * viewMat * modelMat * pos;

        clips.push_back(getExtendedCohenSutherlandCode(newPos, CLIP_LEFT, CLIP_RIGHT,
                                              CLIP_BOTTOM, CLIP_TOP, CLIP_NEAR,
                                              CLIP_FAR));
        //clips.push_back(getExtendedCohenSutherlandCode(newPos));

        outMesh->getVertices().at(i).setPos(Vec3f(newPos.x / newPos.w, newPos.y / newPos.w, newPos.z / newPos.w));
        outMesh->getVertices().at(i).screenTransform(windowWidth, windowHeight);
    }
    outMesh->getFaces().clear();

    int p = 0;
    for (Face &face : inputMesh->getFaces()) {
        ++p;
        bool allIn = true;
        for (int i = 0; i < face.indices.size(); ++i) {
            //try{
            if (clips.at(face.indices.at(i)) != 0) {
                allIn = false;
                break;
            }//} catch(out_of_range){
            //    allIn = false;
           //     cout << "broken " << i << " p " << p << endl;
           //     break;}
        }
        if (allIn) {
            outMesh->getFaces().push_back(face);
        }
    }
    //for(Vert &v : outMesh->getVertices()){
    //    v.screenTransform(windowWidth, windowHeight);
    //}
    //cout << "BREAKPOINT" << endl;
}

