//
//  mtlWatchFolderExampleApp.h
//  ofxMtlWatchFolder
//
//  Created by Elie Zananiri on 12-06-17.
//  Copyright (c) 2012 Departement. All rights reserved.
//

#pragma once

#include "ofMain.h"

#include "ofxMtlWatchFolder.h"

//--------------------------------------------------------------
//--------------------------------------------------------------
class mtlWatchFolderExampleApp : public ofBaseApp
{
public:
    void setup();
    void update();
    void draw();

    void keyPressed(int key);
    void keyReleased(int key);
    void mouseMoved(int x, int y);
    void mouseDragged(int x, int y, int button);
    void mousePressed(int x, int y, int button);
    void mouseReleased(int x, int y, int button);
    void windowResized(int w, int h);
    void dragEvent(ofDragInfo dragInfo);
    void gotMessage(ofMessage msg);
    
    void watchFolderFileAdded(string& filename);
    void watchFolderFileRemoved(string& filename);
    
private:
    ofxMtlWatchFolder watchFolder;
    
    int currY;
    int margin;
    int lineHeight;
};
