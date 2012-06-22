//
//  ofxMtlWatchFolder.h
//  ofxMtlWatchFolder
//
//  Created by Elie Zananiri on 12-06-17.
//  Copyright (c) 2012 Departement. All rights reserved.
//

#pragma once

#include "ofMain.h"

//--------------------------------------------------------------
//--------------------------------------------------------------
struct ofxMtlWatchFile
{
    string name;
    bool flag;
};

//--------------------------------------------------------------
//--------------------------------------------------------------
class ofxMtlWatchFolder : public ofThread
{
public:
    ofxMtlWatchFolder();
    ~ofxMtlWatchFolder();
    
    void start(const string& path, int interval);
    void stop();
    
    void threadedFunction();
    
    ofEvent<string> fileAdded;
    ofEvent<string> fileRemoved;
    
private:
    void checkFolder();
    void update(ofEventArgs& args);
    
    string _watchPath;
    map<string, bool> _watchFiles;
    int _checkInterval;
    
    vector<string> filesAdded;
    vector<string> filesRemoved;
};
