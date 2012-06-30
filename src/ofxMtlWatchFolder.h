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
    bool     done;
    bool     flag;
    uint64_t size;
    unsigned time;
};

//--------------------------------------------------------------
//--------------------------------------------------------------
class ofxMtlWatchFolder : public ofThread
{
public:
    ofxMtlWatchFolder();
    ~ofxMtlWatchFolder();
    
    void allowExt(const string& ext);
    
    void start(const string& path, unsigned checkInterval, unsigned sizeInterval = 3000);
    void stop();
    
    void threadedFunction();
    
    ofEvent<string> fileAdded;
    ofEvent<string> fileRemoved;
    
private:
    void checkFolder();
    void update(ofEventArgs& args);
    
    ofDirectory _watchDir;
    string _watchPath;
    map<string, ofxMtlWatchFile> _watchFiles;
    unsigned _checkInterval;
    unsigned _sizeInterval;
    
    vector<string> filesAdded;
    vector<string> filesRemoved;
};
