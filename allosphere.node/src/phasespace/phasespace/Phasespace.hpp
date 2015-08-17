//
//  Phasespace.hpp
//  Created by Tim Wood, 2013
//

#ifndef _Phasespace_hpp_
#define _Phasespace_hpp_

#include "owl/owl.h"
#include "owl/libowlsock.cc"
#include "owl/owl_math.cc"

#include <allofw/math/pose.h>
// #include "allocore/math/al_Vec.hpp"
// #include "allocore/spatial/al_Pose.hpp"
// #include "allocore/io/al_File.hpp"

typedef allofw::Vector3f Vec3f;
typedef allofw::Pose Pose;
typedef allofw::Quaternionf Quatf;

#include <map>
#include <vector>
#include <fstream>
#include <sstream>

#define SERVER_NAME "192.168.0.99"

#define MARKER_COUNT 180
#define RIGID_COUNT 10

#define PHASESPACE_SCALE 0.001 //scales data to meters
#define OFFSET_Y -.53

using namespace std;

struct Marker {
    Marker(int id_, float x_, float y_, float z_) : id(id_), x(x_), y(y_), z(z_) {}
    int id;
    float x,y,z;
};

struct RigidDef {
    bool active;
    int tracker;
    map<int, float*> markers;

    RigidDef(){};

    void save(string& name){
        // File f = File(name, "rw");
        // stringstream ss;
        // for (map<int,float*>::iterator itr = markers.begin(); itr != markers.end(); ++itr){
        //     int idx = itr->first;
        //     float *pos = itr->second;
        //     ss << idx << " " << pos[0] << " " << pos[1] << " " << pos[2] << endl;
        // }
        // f.write( ss.str() );
        // f.close();
    };
    void load(string& name){
        //TODO
    };
};

class Phasespace {
public:

    Phasespace(){
        for(int i = 0;i<MARKER_COUNT;i++){
            markerPositions[i] = Vec3f(0.0, 0.0, 0.0);
            markerNumberOfAbsentFrames[i] = 99999;
            markerAssigned[i] = false;
        }
        for(int i = 0;i<RIGID_COUNT;i++){
            rigidNumberOfAbsentFrames[i] = 99999;
        }
        mode = 0;
        slave = postProcess = recording = false;
        owlFrequency = 120;
        // oscReceiver = NULL;
        owlRunning = false;
    }

    int start(){
        //initialize
        int initFlags = 0;
        if( slave ) initFlags |= OWL_SLAVE;
        if( postProcess ) initFlags |= OWL_POSTPROCESS;
        switch( mode ){
            case 1: initFlags |= OWL_MODE1; break;
            case 2: initFlags |= OWL_MODE2; break;
            case 3: initFlags |= OWL_MODE3; break;
            case 4: initFlags |= OWL_MODE4; break;
            default: break;
        }

        if (owlInit(SERVER_NAME, initFlags) < 0){
            owl_print_error("Couldn't initialize: \n", owlGetError());
            return -1;
        }
        else{

            // for each rigid body definition
            for( int i=0; i < rigidBodyDefinitions.size(); i++){

                RigidDef* r = rigidBodyDefinitions[i];
                owlTrackeri(r->tracker, OWL_CREATE, OWL_RIGID_TRACKER);

                // set markers to use and offsets
                for (map<int,float*>::iterator itr = r->markers.begin(); itr != r->markers.end(); ++itr){
                    int idx = itr->first;
                    owlMarkeri(MARKER(r->tracker, idx), OWL_SET_LED, idx);
                    owlMarkerfv(MARKER(r->tracker, idx), OWL_SET_POSITION, itr->second);
                }
                owlTracker(r->tracker, OWL_ENABLE);// flush requests and check for errors

                if(!owlGetStatus()){
                    printf("error in rigid setup(%d): %d\n", i, owlGetError());
                    return -1;
                }

            }

            // create remaining markers as point tracker
            int tracker = 0;
            owlTrackeri(tracker, OWL_CREATE, OWL_POINT_TRACKER); // create tracker 0
            for(int i = 0; i < MARKER_COUNT; i++){
                if( !markerAssigned[i] ) owlMarkeri(MARKER(tracker, i), OWL_SET_LED, i);// activate tracker
            }
            owlTracker(tracker, OWL_ENABLE);// flush requests and check for errors

            if(!owlGetStatus()){
                owl_print_error("error in point tracker setup", owlGetError());
                return -1;
            }

            owlSetFloat(OWL_FREQUENCY, owlFrequency);// set default frequency OWL_MAX_FREQUENCY (this is 480)
            owlSetInteger(OWL_STREAMING, OWL_ENABLE);// start streaming

            owlScale(PHASESPACE_SCALE);                 // scale data to meters
            float pose[7] = {0.0, OFFSET_Y, 0.0, 1,0,0,0 }; // translate phasespace origin to match sphere center
            owlLoadPose(pose);
        }

        cout << "Starting Phasespace thread\n";
        run = 1;
        owlRunning = true;
        int pthreadErrorCode = pthread_create(&workerThread, NULL, OwlCallback, NULL);
        if (pthreadErrorCode){
            printf("ERROR; Couldn't start workerThread. Return code from pthread_create() is %d\n", pthreadErrorCode);
            exit(-1);
        }
        return 0;
    }

    void stop(){
        run = 0;
        void* ret;
        pthread_join(workerThread, &ret);
        owlDone();
    }

    void restart(){
        if( !owlRunning ) return;
        stop();
        sleep(1);
        start();
    }

    // void startOSC(int port=8008){
    //         // Receiving packets
    //     struct OSCHandler : public osc::PacketHandler{
    //         void onMessage(osc::Message& m){
    //             //m.print();
    //             string dest = m.addressPattern();
    //             int i;
    //             float x,y,z,qw,qx,qy,qz;

    //             if( dest == "/marker"){
    //                 m >> i >> x >> y >> z;
    //                 master()->updateMarker(i, Vec3f(x,y,z));
    //             } else if( dest == "/rigid"){
    //                 m >> i >> x >> y >> z >> qw >> qx >> qy >> qz;
    //                 master()->updateRigid(i, Pose(Vec3f(x,y,z),Quatf(qw,qx,qy,qz)));
    //             }
    //         }
    //     };

    //     cout << "Starting OSC receive thread\n";

    //     OSCHandler* handler = new OSCHandler();
    //     if( oscReceiver ){
    //      delete oscReceiver;
    //      oscReceiver = NULL;
    //     }

    //     oscReceiver = new osc::Recv(port);
    //     if( !oscReceiver ){
    //         cout << "failed to create OSC recv on port " << port << endl;
    //         return;
    //     }

    //     // Assign a handler to the receiver
    //     oscReceiver->handler(*handler);

    //     // Here we launch a background thread that automatically checks the
    //     // socket for incoming OSC packets.
    //     oscReceiver->timeout(0.1); // set receiver to block with timeout
    //     oscReceiver->start();
    //     // run = 1;
    //     // int pthreadErrorCode = pthread_create(&workerThread, NULL, OSCCallback, NULL);
    //     // if (pthreadErrorCode){
    //     //     printf("ERROR; Couldn't start osc thread. Return code from pthread_create() is %d\n", pthreadErrorCode);
    //     //     exit(-1);
    //     // }
    // }

    void startPlaybackFile(string name){
        playbackFile.open(name.c_str());
        if( !playbackFile.is_open()){
            cout << "Error: failed to read file: " << name << endl;
            return;
        }

        int i;
        float x,y,z;
        playbackFile >> i >> x >> y >> z;
        while( !playbackFile.eof() ){
            Marker m(i,x,y,z);
            playbackData.push_back(m);
            playbackFile >> i >> x >> y >> z;
        }
        playbackFile.close();
        cout << "Read " << playbackData.size() << " markers from " << name << endl;
        run = 1;
        int pthreadErrorCode = pthread_create(&workerThread, NULL, playCallback, NULL);
        if (pthreadErrorCode){
            printf("ERROR; Couldn't start worker thread. Return code from pthread_create() is %d\n", pthreadErrorCode);
            exit(-1);
        }
        cout << "playing marker data.." << endl;
    }

    void startRecordingMarkers(string name){
        // ofstream myfile;
        recordFile.open (name.c_str());
        recording = true;

        // recordFile = new File(name, "rw", true);
        // if( recordFile ){
        //     cout << "Recording marker data.." << endl;
        //     recording = true;
        // } else cout << "Failed to create marker data file" << endl;
    }
    void stopRecordingMarkers(){

        recording = false;
        usleep(200);
        recordFile.close();
        cout << "Recording stopped." << endl;
        //delete recordFile;
    }

    Vec3f* getMarkerPositions(){
        return master()->markerPositions;
    }

    Pose* getRigidPoses(){
        return master()->rigidPoses;
    }

    void getMarkers(Vec3f* positions, int* frames, int offset, int count){
        for(int i = 0; i<count; i++){
            positions[i] = markerPositions[i+offset];
            frames[i] = markerNumberOfAbsentFrames[i+offset];
        }
    }

    void getRigids(Pose* poses, int* frames, int offset, int count){
        for(int i = 0; i<count; i++){
            poses[i] = rigidPoses[i+offset];
            frames[i] = rigidNumberOfAbsentFrames[i+offset];
        }
    }

    void setMode( int _mode ){
        mode = _mode;
        restart();
    }
    void setFrequency( int freq ){
        owlFrequency = freq;
        restart();
    }

    void addRigid( RigidDef *r){
        for (map<int,float*>::iterator it = r->markers.begin(); it != r->markers.end(); ++it){
                markerAssigned[it->first] = true;
        }
        r->active = true;
        rigidBodyDefinitions.push_back(r);
        r->tracker = rigidBodyDefinitions.size();
    }

    int addRigidBodyFromVisibleMarkers(){
        addRigidBodyFromMarkers(0,MARKER_COUNT);
    }

    int addRigidBodyFromMarkers(int offset, int count){ //TODO cleanup dynamically allocated memories on failure..
        RigidDef *r = new RigidDef();
        Vec3f center;
        for(int i=offset; i < offset+count; i++){
            if( markerNumberOfAbsentFrames[i] == 0){
                if( markerAssigned[i] ){
                    cout << "Error: can't use one of seen markers, already assigned to another rigid body!\n";
                    continue;
                }
                float *f = new float[3]();
                Vec3f v = markerPositions[i]*(1.f/PHASESPACE_SCALE);
                center += v;
                f[0] = v.x;
                f[1] = v.y;
                f[2] = v.z;
                r->markers[i] = f;
            }
        }
        if( r->markers.size() == count){
            center /= r->markers.size();
            for (map<int,float*>::iterator it = r->markers.begin(); it != r->markers.end(); ++it){
                float *f = it->second;
                f[0] -= center.x;
                f[1] -= center.y;
                f[2] -= center.z;
                markerAssigned[it->first] = true;
            }
            r->active = true;
            rigidBodyDefinitions.push_back(r);
            r->tracker = rigidBodyDefinitions.size();

            restart();
            return r->markers.size();
        }else{
            cout << "Not enough markers detected..\n";
        }
        delete r;
        return 0;
    }

    static Phasespace* master(){
        static Phasespace *master = new Phasespace;
        return master;
    }

    void updateMarker( int id, Vec3f pos){
        markers[id].cond = 1;
        markers[id].x = pos.x;
        markers[id].y = pos.y;
        markers[id].z = pos.z;
        markerPositions[id] = pos;
        markerNumberOfAbsentFrames[id] = 0;
        if(recording){
            recordFile << id << " " << markers[id].x << " " << markers[id].y << " " << markers[id].z << endl;
        }
    }

    void updateRigid( int id, Pose pose){
        rigidPoses[id] = pose;
        rigidNumberOfAbsentFrames[id] = 0;
    }

private:

    void updateOwlMarkerData(){
        int numberOfMarkers = owlGetMarkers(markers, MARKER_COUNT);// get some markers
        int numberOfRigids = owlGetRigids(rigids, RIGID_COUNT);

        int error = owlGetError();
        if(error != OWL_NO_ERROR){
            owl_print_error("error", error);
            return;
        }
        if(numberOfMarkers > 0){
            for(int i = 0; i < numberOfMarkers; i++){
                if(markers[i].cond > 0){
                    //printf("marker: %d    x: %f, y: %f, z: %f\n", markers[i].id, markers[i].x, markers[i].y, markers[i].z);
                    markerPositions[i] = Vec3f(markers[i].x, markers[i].y, markers[i].z);
                    markerNumberOfAbsentFrames[i] = 0;
                    if(recording){
                        // stringstream ss;
                        recordFile << i << " " << markers[i].x << " " << markers[i].y << " " << markers[i].z << endl;
                        // recordFile->write( ss.str() );
                    }
                } else{
                    markerNumberOfAbsentFrames[i]++;
                }
            }
        }
        if(numberOfRigids > 0){
            for(int i = 0; i < numberOfRigids; i++){
                if(rigids[i].cond > 0){
                    float *p = rigids[i].pose;
                    rigidPoses[i].position = Vec3f(p[0], p[1], p[2]);
                    rigidPoses[i].rotation = Quatf(p[3],p[4],p[5],p[6]);

                    //printf("rigid(%d): %d ", i, rigids[i].id );
                    //rigidPoses[i].print();

                    rigidNumberOfAbsentFrames[i] = 0;
                } else{
                    rigidNumberOfAbsentFrames[i]++;
                }
            }
        }
    }

    static void *OwlCallback(void*){
        while(master()->run == 1){
            master()->updateOwlMarkerData();
        }
        return 0;
    }

    static void *playCallback(void*){
        int idx = 0;
        if(0 == master()->playbackData.size()) return 0;
        while(master()->run == 1){
            Marker m = master()->playbackData[idx++];
            idx %= master()->playbackData.size();
            master()->updateMarker(m.id, Vec3f(m.x,m.y,m.z));
            usleep(10000);
        }
        return 0;
    }

    // void updateOSCMarkerData(){
    //     while( oscReceiver->recv() ){}
    // }
    // static void *OSCCallback(void*){
    //     while(master()->run == 1){
    //         master()->updateOSCMarkerData();
    //     }
    //     return 0;
    // }

    void owl_print_error(const char *s, int n){
        if(n < 0) printf("%s: %d\n", s, n);
        else if(n == OWL_NO_ERROR) printf("%s: No Error\n", s);
        else if(n == OWL_INVALID_VALUE) printf("%s: Invalid Value\n", s);
        else if(n == OWL_INVALID_ENUM) printf("%s: Invalid Enum\n", s);
        else if(n == OWL_INVALID_OPERATION) printf("%s: Invalid Operation\n", s);
        else printf("%s: 0x%x\n", s, n);
    }

public:
//private:
    int run;
    int mode, owlFrequency;
    bool slave, postProcess;
    bool recording;
    bool owlRunning;
    // File* recordFile;
    ofstream recordFile;
    ifstream playbackFile;
    vector<Marker> playbackData;

    pthread_t workerThread;
    // osc::Recv *oscReceiver;

    vector<RigidDef*> rigidBodyDefinitions;

    OWLMarker markers[MARKER_COUNT];
    OWLRigid rigids[RIGID_COUNT];
    bool markerAssigned[MARKER_COUNT];

    Vec3f markerPositions[MARKER_COUNT];
    Pose rigidPoses[RIGID_COUNT];
    int markerNumberOfAbsentFrames[MARKER_COUNT];
    int rigidNumberOfAbsentFrames[RIGID_COUNT];
};

#endif
