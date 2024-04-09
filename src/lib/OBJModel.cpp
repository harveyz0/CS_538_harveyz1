
#include "Exceptions.hpp"
#include "Mesh.hpp"
#include "OBJModel.hpp"
#include <fstream>
#include <sstream>
#include <string>

namespace potato {

    PolyMesh *loadOBJModel(string filename) {
        ifstream inObj;
        inObj.open(filename);
        if (!inObj.is_open()) {
            throw ifstream::failure("Failed to open " + filename);
        }
        string    currentLine;

        PolyMesh *mesh = new PolyMesh();

        while (!inObj.eof()) {
            getline(inObj, currentLine);

            Token curTok = parseSourceLine(currentLine);
            switch (curTok) {
            case Token::Vertex: {
                Vert *v = parseVertex(currentLine);
                 mesh->getVertices().push_back(*v);
                break;
            }
            case Token::Face: {
                Face *f = parseFace(currentLine);
                mesh->getFaces().push_back(*f);
                break;
            }
            case Token::Normals:
                break;
            case Token::Name:
                break;
            case Token::Comment:
            case Token::Whitespace:
            case Token::Empty:
                break;
            case Token::SmoothShading:
                break;
            default:
                throw syntax_error("I do not know what happened : " +
                                   currentLine);
            }
        }
        return mesh;
    }

    Vert* parseVertex(const string &line) { // throws invalid_argument
        stringstream walker(line);
        string       v;
        if (!walker.eof()) {
            // Eat the first section which should be the v character
            walker >> v;
        }
        // If theres no W cordinate than the default is 1, otherwise it'll get
        // overwritten with whatever is in the file.
        vector<string> vals = {"", "", ""};
        int   i    = 0;

        while (i < vals.size() && !walker.eof()) {
            walker >> vals[i];
            ++i;
        }
        if (i < vals.size()) {
            throw syntax_error("Not enough data to build vertex on line [" +
                               line + "]");
        }
        return new Vert(std::stof(vals.at(0)), std::stof(vals.at(1)),
                        std::stof(vals.at(2)));
    }

    Face *parseFace(const string &line) { // throws invalid_argument for stoul
        /*
         * Parse a single line of the form of one of these.
         * f 1 2 3
         * f 3/1 4/2 5/3
         * f 6/4/1 3/5/3 7/6/5
         * f 7//1 8//2 9//3
         */
        stringstream walker(line);
        string       f;
        if (!walker.eof()) {
            walker >> f;
        }
        Face *newFace = new Face();
        while (!walker.eof()) {
            string curIndices;
            walker >> curIndices;
            int currentIndex = 0;
            stringstream currentNumber;
            for (int i = 0; i < curIndices.size(); ++i) {
                if (curIndices.at(i) == '/') {
                    if (currentNumber.str() != "") {
                        unsigned int num = std::stoi(currentNumber.str());
                        num = num - 1;
                        switch (currentIndex) {
                        case 0:
                            newFace->indices.push_back(num);
                            break;
                        case 1:
                            newFace->textureIndices.push_back(num);
                            break;
                        case 2:
                            newFace->normalIndices.push_back(num);
                            break;
                        default:
                            throw syntax_error(
                                "I do not know how you got here " +
                                std::to_string(currentIndex) + " " +
                                curIndices);
                        }
                    }
                    currentNumber.str("");
                    ++currentIndex;
                } else {
                    currentNumber << curIndices.at(i);
                }
            }
        }
        return newFace;
    }

    Token parseSourceLine(const string &line) {
        stringstream current(line);
        while (!current.eof()) {
            string token;
            current >> token;
            if (token == "#") {
                return Token::Comment; // It's a comment so just bail out and go
                                       // to the next line.
            } else if (token == "v") {
                return Token::Vertex;
            } else if (token == "f") {
                return Token::Face;
            } else if (token == "o") {
                return Token::Name;
            } else if (token == "vn") {
                return Token::Normals;
            } else if (token == "s") {
                return Token::SmoothShading;
            } else if (token == "") {
                return Token::Empty;
            } else {
                throw not_implemented("Token [" + token +
                                      "] has not been implemented");
            }
        }
        return Token::Empty; // If you got here then the line is empty.
    }

}; // namespace potato
