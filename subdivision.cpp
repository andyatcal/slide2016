/**
 * @author Andy Wang, UC Berkeley.
 * Copyright 2016 reserve.
 * UC Berkeley, Slide_2016 project.
 * Advised by Prof. Sequin H. Carlos.
 */


#include "subdivison.h"

Subdivision::Subdivision(Mesh mesh){
    currMesh = mesh;
}

void Subdivision::makeFacePoints(vector<Vertex*> &newVertList) {
    vector<Face*>::iterator fIt;
    for(fIt = currMesh.faceList.begin(); fIt < currMesh.faceList.end(); fIt++){
        Vertex * newFacePoint = new Vertex;
        vec3 newFacePointPosition = vec3(0, 0, 0);
        Face * currFace = (*fIt);
        Edge * firstEdge = currFace -> oneEdge;
        if(firstEdge == NULL) {
            cout<<"ERROR: This face (with ID) does not have a sideEdge."<<endl;
            exit(1);
        }
        Edge * currEdge = firstEdge;
        uint counter = 0;
        Vertex * currVert;
        do {
            if(currFace == currEdge -> fa) {
                currVert = currEdge -> vb;
                currEdge = currEdge -> nextVbFa;
            } else if(currFace == currEdge -> fb) {
                if(currEdge -> mobius) {
                    currVert = currEdge -> vb;
                    currEdge = currEdge -> nextVbFb;
                } else {
                    currVert = currEdge -> va;
                    currEdge = currEdge -> nextVaFb;
                }
            }
            newFacePointPosition += currVert -> position;
            counter += 1;
        } while (currEdge != firstEdge);
        newFacePointPosition /= counter;
        newFacePoint -> position = newFacePointPosition;
        newFacePoint -> ID = newVertList.size();
        //cout<<"New Face Point: ID: "<< newFacePoint -> ID <<" Position: "<< (newFacePoint -> position)[0]<<" "<<(newFacePoint -> position)[1]<<" "<<(newFacePoint -> position)[2]<<endl;
        currFace -> facePoint = newFacePoint;
        newVertList.push_back(newFacePoint);
    }
}

void Subdivision::makeEdgePoints(vector<Vertex*> &newVertList) {
    vector<Face*>::iterator fIt;
    for(fIt = currMesh.faceList.begin(); fIt < currMesh.faceList.end(); fIt++){
        Face * currFace = (*fIt);
        Edge * firstEdge = currFace -> oneEdge;
        Edge * currEdge = firstEdge;
        Vertex * currVert;
        do {
            Vertex * newEdgePoint = new Vertex;
            if(currEdge -> edgePoint == NULL) {
                if(currEdge -> isSharp) {
                    newEdgePoint -> position = (currEdge -> va -> position +
                     currEdge -> vb -> position) / (float) 2.0;
                } else {
                    Vertex * faceVert1 = currEdge -> fa -> facePoint;
                    Vertex * edgeVert1 = currEdge -> va;
                    Vertex * edgeVert2 = currEdge -> vb;
                    Vertex * faceVert2 = currEdge -> fb -> facePoint;
                    newEdgePoint -> position = (faceVert1 -> position + faceVert2 -> position
                     + edgeVert1 -> position + edgeVert2 -> position) / (float) 4.0;
                }
                currEdge -> edgePoint = newEdgePoint;
                newEdgePoint -> ID = newVertList.size();
                newVertList.push_back(newEdgePoint);
                //cout<<"New Edge Point: ID: "<< newEdgePoint -> ID <<" Position: "<< (newEdgePoint -> position)[0]<<" "<<(newEdgePoint -> position)[1]<<" "<<(newEdgePoint -> position)[2]<<endl;
            }
            if(currFace == currEdge -> fa) {
                currVert = currEdge -> vb;
                currEdge = currEdge -> nextVbFa;
            } else if(currFace == currEdge -> fb) {
                if(currEdge -> mobius) {
                    currVert = currEdge -> vb;
                    currEdge = currEdge -> nextVbFb;
                } else {
                    currVert = currEdge -> va;
                    currEdge = currEdge -> nextVaFb;
                }
            }
        } while (currEdge != firstEdge);
    }
}

void Subdivision::makeVertexPointsC(vector<Vertex*> &newVertList){
    vector<Vertex*>::iterator vIt;
    Vertex facePointAvg;
    Vertex edgePointAvg;
    Vertex * currVert;
    Vertex * newVertexPoint;
    for(vIt = currMesh.vertList.begin(); vIt < currMesh.vertList.end(); vIt++){
        //cout<<"New Vertex!"<<endl;
        currVert = (*vIt);
        newVertexPoint = new Vertex;
        //cout<<"vertexID: "<<currVert -> ID<<endl;
        Edge * firstEdge = currVert -> oneEdge;
        Edge * currEdge = firstEdge;
        Face * currFace = currEdge ->fa;
        int sharpEdgeCounter = 0;
        Edge * sharpEdgeI;
        Edge * sharpEdgeK;
        vec3 facePointAvgPosition = vec3(0, 0, 0);
        vec3 midPointAvgPoistion = vec3(0, 0, 0);
        int n = 0;
        do {
            //cout<<"Now the sharp edge counter is "<<sharpEdgeCounter<<endl;
            //cout<<"here"<<endl<<nextOutEdge -> end -> ID<<endl;
            midPointAvgPoistion += (currEdge -> va -> position
             + currEdge -> vb -> position) / 2.0f;
            facePointAvgPosition += currFace -> facePoint -> position;
            n += 1;
            if(currEdge -> isSharp) {
                //cout<<"A"<<endl;
                sharpEdgeCounter += 1;
                if(sharpEdgeCounter == 1) {
                    sharpEdgeI = currEdge;
                } else if(sharpEdgeCounter == 2) {
                    sharpEdgeK = currEdge;
                }
                currFace = currEdge -> theOtherFace(currFace);
                if(currFace == NULL) {
                    //cout<<"A1"<<endl;
                    currEdge = currEdge -> nextEdge(currVert, currFace);
                    currFace = currEdge -> theOtherFace(currFace);
                    midPointAvgPoistion += (currEdge -> va -> position
                     + currEdge -> vb -> position) / 2.0f;
                    sharpEdgeCounter += 1;
                    if(sharpEdgeCounter == 2) {
                        sharpEdgeK = currEdge;
                    }
                }
                currEdge = currEdge -> nextEdge(currVert, currFace);
            } else {
                currFace = currEdge -> theOtherFace(currFace);
                currEdge = currEdge -> nextEdge(currVert, currFace);
            }
        } while ( currEdge != firstEdge);
        if(sharpEdgeCounter <= 1) {
            facePointAvgPosition /= n;
            midPointAvgPoistion /= n;
            newVertexPoint -> position = ((float) (n - 3) * currVert -> position
             + 2.0f * midPointAvgPoistion + facePointAvgPosition) / (float) n;
            //cout<<"this is a normal vertex! "<<newVertexPoint -> position[0] << newVertexPoint -> position [1]<< newVertexPoint -> position[2]<<endl;
        } else if(sharpEdgeCounter == 2) {
            Vertex * pointI = sharpEdgeI -> theOtherVertex(currVert);
            Vertex * pointK = sharpEdgeK -> theOtherVertex(currVert);
            newVertexPoint -> position = (pointI -> position + pointK -> position
             + 6.0f * currVert -> position) / 8.0f;
            //cout<<"this is a crease vertex! "<<newVertexPoint -> position[0] << newVertexPoint -> position [1]<< newVertexPoint -> position[2]<<endl;;
        } else {
            newVertexPoint -> position = currVert -> position;
            //cout<<"this is a conner vertex! "<<newVertexPoint -> position[0] << newVertexPoint -> position [1]<< newVertexPoint -> position[2]<<endl;
        }
        newVertexPoint -> ID = newVertList.size();
        currVert -> vertexPoint = newVertexPoint;
        newVertList.push_back(newVertexPoint);
        //cout<<"New Vertex Point: ID: "<< newVertexPoint -> ID <<" Position: "<< (newVertexPoint -> position)[0]<<" "<<(newVertexPoint -> position)[1]<<" "<<(newVertexPoint -> position)[2]<<endl;
    }
}

void Subdivision::makeVertexPointsD(vector<Vertex*> &newVertList){
    vector<Vertex*>::iterator vIt;
    Vertex * currVert;
    Vertex * newVertexPoint;
    for(vIt = currMesh.vertList.begin(); vIt < currMesh.vertList.end(); vIt++){
        //cout<<"New Vertex!"<<endl;
        currVert = (*vIt);
        newVertexPoint = new Vertex;
        //cout<<"vertexID: "<<currVert -> ID<<endl;
        Edge * firstEdge = currVert -> oneEdge;
        Edge * currEdge = firstEdge;
        Face * currFace = currEdge ->fa;
        int sharpEdgeCounter = 0;
        Edge * sharpEdgeI;
        Edge * sharpEdgeK;
        vec3 facePointAvgPosition = vec3(0, 0, 0);
        vec3 edgePointAvgPoistion = vec3(0, 0, 0);
        int n = 0;
        do {
            //cout<<"Now the sharp edge counter is "<<sharpEdgeCounter<<endl;
            //cout<<"here"<<endl<<nextOutEdge -> end -> ID<<endl;
            edgePointAvgPoistion += currEdge -> edgePoint -> position;
            facePointAvgPosition += currFace -> facePoint -> position;
            n += 1;
            if(currEdge -> isSharp) {
                //cout<<"A"<<endl;
                sharpEdgeCounter += 1;
                if(sharpEdgeCounter == 1) {
                    sharpEdgeI = currEdge;
                } else if(sharpEdgeCounter == 2) {
                    sharpEdgeK = currEdge;
                }
                currFace = currEdge -> theOtherFace(currFace);
                if(currFace == NULL) {
                    //cout<<"A1"<<endl;
                    currEdge = currEdge -> nextEdge(currVert, currFace);
                    currFace = currEdge -> theOtherFace(currFace);
                    edgePointAvgPoistion += currEdge -> edgePoint -> position;
                    sharpEdgeCounter += 1;
                    if(sharpEdgeCounter == 2) {
                        sharpEdgeK = currEdge;
                    }
                }
                currEdge = currEdge -> nextEdge(currVert, currFace);
            } else {
                currFace = currEdge -> theOtherFace(currFace);
                currEdge = currEdge -> nextEdge(currVert, currFace);
            }
        } while ( currEdge != firstEdge);
        if(sharpEdgeCounter <= 1) {
            facePointAvgPosition /= n;
            edgePointAvgPoistion /= n;
            newVertexPoint -> position = ((float) (n - 2) * currVert -> position
             + edgePointAvgPoistion + facePointAvgPosition) / (float) n;
            //cout<<"this is a normal vertex! "<<newVertexPoint -> position[0] << newVertexPoint -> position [1]<< newVertexPoint -> position[2]<<endl;
        } else if(sharpEdgeCounter == 2) {
            Vertex * pointI = sharpEdgeI -> theOtherVertex(currVert);
            Vertex * pointK = sharpEdgeK -> theOtherVertex(currVert);
            newVertexPoint -> position = (pointI -> position + pointK -> position
             + 6.0f * currVert -> position) / 8.0f;
            //cout<<"this is a crease vertex! "<<newVertexPoint -> position[0] << newVertexPoint -> position [1]<< newVertexPoint -> position[2]<<endl;;
        } else {
            newVertexPoint -> position = currVert -> position;
            //cout<<"this is a conner vertex! "<<newVertexPoint -> position[0] << newVertexPoint -> position [1]<< newVertexPoint -> position[2]<<endl;
        }
        newVertexPoint -> ID = newVertList.size();
        currVert -> vertexPoint = newVertexPoint;
        newVertList.push_back(newVertexPoint);
        //cout<<"New Vertex Point: ID: "<< newVertexPoint -> ID <<" Position: "<< (newVertexPoint -> position)[0]<<" "<<(newVertexPoint -> position)[1]<<" "<<(newVertexPoint -> position)[2]<<endl;
    }
}

void Subdivision::compileNewMesh(vector<Face*> &newFaceList){
    vector<Face*>::iterator fIt;
    for(fIt = currMesh.faceList.begin(); fIt < currMesh.faceList.end(); fIt++){
        Face * currFace = (*fIt);
        Edge * firstEdge = currFace -> oneEdge;
        Edge * currEdge = firstEdge;
        Edge * nextEdge;
        Edge * previousB;
        Edge * previousEF;
        Edge * edgeA;
        Edge * edgeB;
        Edge * edgeEF;
        Face * newFace;
        bool notFirstFace = false;
        // Split the edges and create "in and out" edges.`
        do{
            newFace = new Face;
            // Create edge and set va and vb
            if(currEdge -> firstHalf == NULL) {
                Edge * newFirstHalf = new Edge;
                Edge * newSecondHalf = new Edge;
                newFirstHalf -> va = currEdge -> va -> vertexPoint;
                newFirstHalf -> vb = currEdge -> edgePoint;
                newSecondHalf -> va = currEdge -> edgePoint;
                newSecondHalf -> vb = currEdge -> vb -> vertexPoint;
                currEdge -> firstHalf = newFirstHalf;
                currEdge -> secondHalf = newSecondHalf;
                newFirstHalf -> va -> oneEdge = newFirstHalf;
                newSecondHalf -> vb -> oneEdge = newSecondHalf;
                currEdge -> edgePoint -> oneEdge = newFirstHalf;
            }
            edgeEF = new Edge;
            edgeEF -> va = currEdge -> edgePoint;
            edgeEF -> vb = currFace -> facePoint;
            if(notFirstFace) {
                edgeEF -> fa = newFace;
                previousEF -> fb = newFace;
            }
            if(currFace == currEdge -> fa) {
                edgeA = currEdge -> firstHalf;
                edgeB = currEdge -> secondHalf;
                edgeA -> nextVbFa = edgeEF;
                edgeB -> nextVaFa = edgeEF;
                if(notFirstFace) {
                    edgeA -> fa = newFace;
                    edgeA -> nextVaFa = previousB;
                    if(previousB -> vb == edgeA -> va) {
                        if(previousB -> mobius) {
                            previousB -> nextVbFb = edgeA;
                            previousB -> fb = newFace;
                        } else {
                            previousB -> nextVbFa = edgeA;
                            previousB -> fa = newFace;
                        }
                    } else {
                        previousB -> nextVaFb = edgeA;
                        previousB -> fb = newFace;
                    }
                }
                nextEdge = currEdge -> nextVbFa;
                if(currEdge -> fb == NULL) {
                    edgeA -> nextVbFb = edgeB;
                    edgeB -> nextVaFb = edgeA;
                    Edge * neighbourboundaryA = currEdge -> nextVaFb;
                    Edge * neighbourboundaryB = currEdge -> nextVbFb;
                    if(neighbourboundaryA -> firstHalf != NULL && edgeA -> nextVaFb == NULL) {
                        if(neighbourboundaryA -> vb == currEdge -> va) {
                            edgeA -> nextVaFb = neighbourboundaryA -> secondHalf;
                            neighbourboundaryA -> secondHalf -> nextVbFb = edgeA;
                        } else {
                            edgeA -> nextVaFb = neighbourboundaryA -> firstHalf;
                            neighbourboundaryA -> firstHalf -> nextVaFb = edgeA;
                        }
                    }
                    if(neighbourboundaryB -> firstHalf != NULL && edgeB -> nextVbFb == NULL) {
                        if(neighbourboundaryB -> va == currEdge -> vb) {
                            edgeB -> nextVbFb = neighbourboundaryB -> firstHalf;
                            neighbourboundaryB -> firstHalf -> nextVaFb = edgeB;
                        } else {
                            edgeB -> nextVbFb = neighbourboundaryB -> secondHalf;
                            neighbourboundaryB -> secondHalf -> nextVbFb = edgeB;
                        }
                    }
                }
            } else {
                if(currEdge -> mobius) {
                    edgeA = currEdge -> firstHalf;
                    edgeB = currEdge -> secondHalf;
                    edgeA -> mobius = true;
                    edgeB -> mobius = true;
                    edgeA -> va -> onMobius = true;
                    edgeB -> vb -> onMobius = true;
                    edgeA -> vb -> onMobius = true;
                    nextEdge = currEdge -> nextVbFb;
                    edgeA -> nextVbFb = edgeEF;
                    edgeB -> nextVaFb = edgeEF;
                    if(notFirstFace) {
                        edgeA -> fb = newFace;
                        edgeA -> nextVaFb = previousB;
                        if(previousB -> vb == edgeA -> va) {
                            if(previousB -> mobius) {
                                previousB -> nextVbFb = edgeA;
                                previousB -> fb = newFace;
                            } else {
                                previousB -> nextVbFa = edgeA;
                                previousB -> fa = newFace;
                            }
                        } else {
                            previousB -> nextVaFb = edgeA;
                            previousB -> fb = newFace;
                        }
                    }
                } else {
                    edgeA = currEdge -> secondHalf;
                    edgeB = currEdge -> firstHalf;
                    nextEdge = currEdge -> nextVaFb;
                    edgeA -> nextVaFb = edgeEF;
                    edgeB -> nextVbFb = edgeEF;
                    if(notFirstFace) {
                        edgeA -> fb = newFace;
                        edgeA -> nextVbFb = previousB;
                        if(previousB -> vb == edgeA -> vb) {
                            if(previousB -> mobius) {
                                previousB -> nextVbFb = edgeA;
                                previousB -> fb = newFace;
                            } else {
                                previousB -> nextVbFa = edgeA;
                                previousB -> fa = newFace;
                            }
                        } else {
                            previousB -> nextVaFb = edgeA;
                            previousB -> fb = newFace;
                        }
                    }
                }
            }
            if(currEdge -> isSharp) {
                edgeA -> isSharp = true;
                edgeB -> isSharp = true;
            }
            edgeEF -> nextVaFa = edgeA;
            edgeEF -> nextVaFb = edgeB;
            if(notFirstFace) {
                edgeEF -> nextVbFa = previousEF;
                previousEF -> nextVbFb = edgeEF;
            }
            currEdge = nextEdge;
            previousB = edgeB;
            previousEF = edgeEF;
            if(notFirstFace) {
                newFace -> oneEdge = edgeA;
                newFace -> id = newFaceList.size();
                newFaceList.push_back(newFace);
            }
            notFirstFace = true;
        } while (currEdge != firstEdge);
        newFace = new Face;
        previousEF -> fb = newFace;
        if(currFace == currEdge -> fa) {
            edgeA = currEdge -> firstHalf;
            edgeEF = edgeA -> nextVbFa;
            edgeA -> fa = newFace;
            edgeA -> nextVaFa = previousB;
            if(previousB -> vb == edgeA -> va) {
                if(previousB -> mobius) {
                    previousB -> nextVbFb = edgeA;
                    previousB -> fb = newFace;
                } else {
                    previousB -> nextVbFa = edgeA;
                    previousB -> fa = newFace;
                }
            } else {
                previousB -> nextVaFb = edgeA;
                previousB -> fb = newFace;
            }
        } else {
            if(currEdge -> mobius) {
                edgeA = currEdge -> firstHalf;
                edgeEF = edgeA -> nextVbFb;
                edgeA -> fb = newFace;
                edgeA -> nextVaFb = previousB;
                if(previousB -> vb == edgeA -> va) {
                    if(previousB -> mobius) {
                        previousB -> nextVbFb = edgeA;
                        previousB -> fb = newFace;
                    } else {
                        previousB -> nextVbFa = edgeA;
                        previousB -> fa = newFace;
                    }
                } else {
                    previousB -> nextVaFb = edgeA;
                    previousB -> fb = newFace;
                }
            } else {
                edgeA = currEdge -> secondHalf;
                edgeEF = edgeA -> nextVaFb;
                edgeA -> fb = newFace;
                edgeA -> nextVbFb = previousB;
                if(previousB -> vb == edgeA -> vb) {
                    if(previousB -> mobius) {
                        previousB -> nextVbFb = edgeA;
                        previousB -> fb = newFace;
                    } else {
                        previousB -> nextVbFa = edgeA;
                        previousB -> fa = newFace;
                    }
                } else {
                    previousB -> nextVaFb = edgeA;
                    previousB -> fb = newFace;
                }
            }
        }
        edgeEF -> nextVbFa = previousEF;
        edgeEF -> fa = newFace;
        previousEF -> nextVbFb = edgeEF;
        newFace -> oneEdge = edgeA;
        newFace -> id = newFaceList.size();
        newFaceList.push_back(newFace);
        currFace -> facePoint -> oneEdge = previousEF;
    }
}

Mesh Subdivision::ccSubdivision(int level){
    Mesh newMesh;
    for(int i = 0; i < level; i++) {
        makeFacePoints(newMesh.vertList);
        makeEdgePoints(newMesh.vertList);
        makeVertexPointsD(newMesh.vertList);
        compileNewMesh(newMesh.faceList);
        setAllNewPointPointersToNull();
        currMesh = newMesh;
        newMesh.clear();
    }
    return currMesh;
}

void Subdivision::setAllNewPointPointersToNull() {
    for(Vertex* v : currMesh.vertList) {
        v -> vertexPoint = NULL;
    }
    for(Face* f : currMesh.faceList) {
        f -> facePoint = NULL;
        Edge * firstEdge = f -> oneEdge;
        Edge * currEdge = firstEdge;
        Vertex * currVert;
        do {
            currEdge -> edgePoint = NULL;
            currEdge -> firstHalf = NULL;
            currEdge -> secondHalf = NULL;
            if(f == currEdge -> fa) {
                currVert = currEdge -> vb;
                currEdge = currEdge -> nextVbFa;
            } else if(f == currEdge -> fb) {
                if(currEdge -> mobius) {
                    currVert = currEdge -> vb;
                    currEdge = currEdge -> nextVbFb;
                } else {
                    currVert = currEdge -> va;
                    currEdge = currEdge -> nextVaFb;
                }
            }
        } while (currEdge != firstEdge);
    }
}

