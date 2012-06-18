//
//  mtlWatchFolderExampleApp.cpp
//  ofxMtlWatchFolder
//
//  Created by Elie Zananiri on 12-06-17.
//  Copyright (c) 2012 Departement. All rights reserved.
//

#include "mtlWatchFolderExampleApp.h"

//--------------------------------------------------------------
void mtlWatchFolderExampleApp::setup()
{
    ofSetBackgroundColor(0);
    ofSetBackgroundAuto(false);
    ofBackground(0);
    
    // start watching the bin/data/watch/ folder every 2000 ms (2 secs)
    watchFolder.start("watch", 2000);
    ofAddListener(watchFolder.fileAdded, this, &mtlWatchFolderExampleApp::watchFolderFileAdded);
    ofAddListener(watchFolder.fileRemoved, this, &mtlWatchFolderExampleApp::watchFolderFileRemoved);
    
    margin = 50;
    lineHeight = 20;
    currY = margin;
}

//--------------------------------------------------------------
void mtlWatchFolderExampleApp::update()
{

}

//--------------------------------------------------------------
void mtlWatchFolderExampleApp::draw()
{

}

//--------------------------------------------------------------
void mtlWatchFolderExampleApp::watchFolderFileAdded(string& filename)
{
    cout << "File '" << filename << "' ADDED!" << endl;
    ofSetColor(255);
    ofDrawBitmapString("File '" + filename + "' ADDED!", margin, currY);
    currY += lineHeight;
    if ((currY + lineHeight) > (ofGetHeight() - margin)) {
        keyPressed(' ');
    }
}

//--------------------------------------------------------------
void mtlWatchFolderExampleApp::watchFolderFileRemoved(string& filename)
{
    cout << "File '" << filename << "' REMOVED!" << endl;
    ofSetColor(255, 0, 0);
    ofDrawBitmapString("File '" + filename + "' REMOVED!", margin, currY);
    currY += lineHeight;
    if ((currY + lineHeight) > (ofGetHeight() - margin)) {
        keyPressed(' ');
    }
}

//--------------------------------------------------------------
void mtlWatchFolderExampleApp::keyPressed(int key)
{
    if (key == ' ') {
        // clear and restart
        currY = margin;
        ofBackground(0);
    }
}

//--------------------------------------------------------------
void mtlWatchFolderExampleApp::keyReleased(int key)
{

}

//--------------------------------------------------------------
void mtlWatchFolderExampleApp::mouseMoved(int x, int y)
{

}

//--------------------------------------------------------------
void mtlWatchFolderExampleApp::mouseDragged(int x, int y, int button)
{

}

//--------------------------------------------------------------
void mtlWatchFolderExampleApp::mousePressed(int x, int y, int button)
{

}

//--------------------------------------------------------------
void mtlWatchFolderExampleApp::mouseReleased(int x, int y, int button)
{

}

//--------------------------------------------------------------
void mtlWatchFolderExampleApp::windowResized(int w, int h)
{

}

//--------------------------------------------------------------
void mtlWatchFolderExampleApp::gotMessage(ofMessage msg)
{

}

//--------------------------------------------------------------
void mtlWatchFolderExampleApp::dragEvent(ofDragInfo dragInfo)
{ 

}
