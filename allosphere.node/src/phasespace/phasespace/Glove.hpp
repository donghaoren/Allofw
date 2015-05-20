//
//  al_PhaseSpaceGlove.hpp
//  by Tim Wood, 2013
//


#ifndef _Phasespace_Glove_hpp_
#define _Phasespace_Glove_hpp_

#include "owl/owl.h"

#include "allocore/math/al_Vec.hpp"
#include "allocore/math/al_Quat.hpp"
// #include "allocore/graphics/al_Graphics.hpp"
#include "allocore/spatial/al_Pose.hpp"

using namespace al;

#define PINCH_THRESH 0.04 //0.049
#define PINCH_SPEED 0.5

// id offset corresponding to glove leds
enum GloveLedID {
    ePinky = 0,
    eRing = 1,
    eMiddle = 2,
    eIndex = 3,
    eBackPinkySide = 4,
    eBackThumbSide = 5,
    eThumbBase = 6,
    eThumbTip = 7
};

struct Glove {

  OWLMarker *markers;
  int offset;
  bool seen[8];
  int numSeen;
  Vec3f pos[8];
  Vec3f oldPos[8];
  Vec3f posSpherical[8];
  Vec3f oldPosSpherical[8];
  Vec3f centroid;
  Vec3f thumb, back;

  Vec3f pinchPos[4];
  Vec3f pinchVel[4];
  Vec3f pinchVelSpherical[4];
  Quatf pinchQuat[4];

  int pinchRigid3rdIndx[4];
  Vec3f pinchRigid[4][3];

  bool wasPinched[4];
  bool pinched[4];
  bool pinchOn[4];
  bool pinchOff[4];

  int pinches[4];
  double dtLastPinch[4];

  float pinchThresh;
  float doublePinchSpeed;

  // default constructor - must call monitorMarkers before using
  Glove(){
    pinchThresh = PINCH_THRESH;
    doublePinchSpeed = PINCH_SPEED;
  }

  // constructor initializes glove by passing in markers and marker offset id
  // usage:
  //   new Glove(PhasespaceManager::master()->markers(),0) )  // glove using markers 0-7
  Glove(OWLMarker *markers_, int offset_){
    pinchThresh = PINCH_THRESH;
    doublePinchSpeed = PINCH_SPEED;
    monitorMarkers(markers_, offset_);
  }

  void monitorMarkers(OWLMarker *markers_, int offset_){
    offset = offset_;
    markers = markers_;
  }

  // updates glove position and pinch detection
  void step(double dt){

    Vec3f sum(0.f);
    numSeen=0;
    
    // set position vectors from marker data
    for( int i=0; i<8; i++ ){
      int indx = i + offset;

      seen[i] = markers[indx].cond > 0.f;

      if( seen[i] ){
        oldPos[i] = pos[i];
        pos[i] = Vec3f(markers[indx].x, markers[indx].y, markers[indx].z); 
        sum += pos[i];
        numSeen++;

        // also calculate spherical coordinates, because.. :p
        // oldPosSpherical[i] = posSpherical[i];
        // posSpherical[i].set( pos[i] );
        // cartToSpherical(posSpherical[i].elems());

        // printf("marker: %d cond: %f x: %f, y: %f, z: %f\n", markers[indx].id, markers[indx].cond, markers[indx].x, markers[indx].y, markers[indx].z);

      }
    }
    if( numSeen > 0 ) centroid = sum / numSeen; 

    // update thumb and back of hand vectors
    if( seen[eThumbTip] && seen[eThumbBase] ) thumb = (pos[eThumbTip] - pos[eThumbBase]).normalize();
    if( seen[eBackThumbSide] && seen[eBackPinkySide] ) back = (pos[eBackPinkySide] - pos[eBackThumbSide]).normalize();

    // update pinched gestures
    for( int i=0; i<4; i++){

      pinchOn[i] = false;
      pinchOff[i] = false;

      // only if thumb and finger both seen
      if( seen[eThumbTip] && seen[i]){
        wasPinched[i] = pinched[i];
        pinched[i] = dist(pos[i], pos[eThumbTip]) < pinchThresh;
        // if( i==3) std::cout << pinchThresh << " " << (dist(pos[i], pos[eThumbTip]) < pinchThresh) << std::endl;
        // if(i==0) std::cout << dist(pos[i], pos[eThumbTip]) << std::endl;
        
        // new pinch
        if(!wasPinched[i] && pinched[i]){
          pinchPos[i] = (pos[i]+pos[eThumbTip]) * 0.5;
          pinchOn[i] = true;

          if( seen[eThumbBase]){
            pinchRigid3rdIndx[i] = eThumbBase;
            pinchRigid[i][0] = pos[i];
            pinchRigid[i][1] = pos[eThumbTip];
            pinchRigid[i][2] = pos[eThumbBase];
          } else pinchRigid3rdIndx[i] = -1;

          if( dtLastPinch[i] < doublePinchSpeed) pinches[i] += 1;
          else pinches[i]=1;
          dtLastPinch[i] = 0.0;

        }else if( pinched[i] ){  // currently pinched
            pinchVel[i].lerp( (pos[i] - oldPos[i]) / dt, 0.2f );

            // Vec3f sv = (posSpherical[i] - oldPosSpherical[i]);
            // if( sv.y > M_PI ) sv.y -= 2*M_PI;
            // if( sv.y < -M_PI ) sv.y += 2*M_PI;
            // if( sv.z > M_PI ) sv.z -= 2*M_PI;
            // if( sv.z < -M_PI ) sv.z += 2*M_PI;
            // pinchVelSpherical[i].lerp( sv / dt, 0.2f );
        }else if( wasPinched[i] ){ // no longer pinched
            pinchOff[i] = true;
        }
      }
      if( dtLastPinch[i] >= doublePinchSpeed) pinches[i] = 0;
      dtLastPinch[i] += dt;

    }
  }

  // position of marker
  Vec3f getPosition(int id){
    if( id < 0 || id > 7) return Vec3f();
    return pos[id];
  }

  // average position of all seen markers
  Vec3f getCentroid(){
    return centroid;
  }

  // id should be 0-3 corresponding to pinching finger
  bool isPinched(int id){
    if( id < 0 || id > 3) return false;
    return pinched[id];
  }
  
  // initial position of pinch
  Vec3f getPinchPosition(int id){
    if( id < 0 || id > 3) return Vec3f();
    return pinchPos[id];
  }

  // smoothed velocity of moving pinch
  Vec3f getPinchVelocity( int id ){
    if( id < 0 || id > 3) return Vec3f();
    return pinchVel[id];
  }

  // vector from initial pinch position to current location
  // if singleAxis is true returns only axis of greatest variation
  Vec3f getPinchTranslate(int id, bool singleAxis=false){ 
    Vec3f vec;
    if( id < 0 || id > 3) return vec;
    if( !pinched[id] ) return vec;
    vec = ((pos[id]+pos[eThumbTip]) * 0.5) - pinchPos[id];

    if( singleAxis){
      if( abs(vec.x) > abs(vec.y))
        if( abs(vec.x) > abs(vec.z))
          return Vec3f(vec.x,0,0);
        else
          return Vec3f(0,0,vec.z);
      else
        if( abs(vec.y) > abs(vec.z))
          return Vec3f(0,vec.y,0);
        else
          return Vec3f(0,0,vec.z);
    }
    return vec;  
  }
  
  Quatf getPinchRotation(int id, bool singleAxis=false){
    // TODO get this working
    Quatf quat;
    if( id < 0 || id > 3) return quat;
    if( !pinched[id] ) return quat;
    if( pinchRigid3rdIndx[id] < 0) return quat;

    // Vec3f s[3];
    // Vec3f t[3];

    // s[0] = pinchRigid[id][0];
    // s[1] = pinchRigid[id][1];
    // s[2] = pinchRigid[id][2];

    // t[0] = pos[id];
    // t[1] = pos[eThumbTip];
    // t[2] = pos[pinchRigid3rdIndx[id]];

    // Vec3f sz = (s[2]-s[1]).normalize();
    // Vec3f sy = cross( (s[0]-s[1]), sz).normalize();
    // Vec3f sx = cross( sy, sz).normalize();

    // Vec3f tz = (t[2]-t[1]).normalize();
    // Vec3f ty = cross( (t[0]-t[1]), sz).normalize();
    // Vec3f tx = cross( ty, tz).normalize();

    // Mat4f m;
    // float v[16];
    // v[0] = tx.x;
    // v[1] = tx.y;
    // v[2] = tx.z;
    // v[3] = 0;
    // v[4] = ty.x;
    // v[5] = ty.y;
    // v[6] = ty.z;
    // v[7] = 0;
    // v[8] = tz.x;
    // v[9] = tz.y;
    // v[10] = tz.z;
    // v[11] = 0;
    // v[12] = v[13] = v[14] = v[15] = 0;
    // m.set(v);

    // return quat.fromMatrix(m);


    Vec3f s1,s2,s3;
    Vec3f t1,t2,t3;

    s1 = pinchRigid[id][0];
    s2 = pinchRigid[id][1];
    s3 = pinchRigid[id][2];

    t1 = pos[id];
    t2 = pos[eThumbTip];
    t3 = pos[pinchRigid3rdIndx[id]];

    Vec3f snorm = cross( (s1-s2), (s1-s3));
    Vec3f tnorm = cross( (t1-t2), (t1-t3));

    Quatf q1 = Quatf::getRotationTo(snorm,tnorm);
    Quatf q2 = Quatf::getRotationTo(q1.rotate(s1-s2),(t2-t1));
    quat = q2 * q1;

    return quat;
  }

  // set nav based on pinch gesture
  //   relative == true -> move nav in direction of pinch translate vector
  //   relative == false -> move nav to current pinch location
  void updateNavFromPinch(Nav &nav, int id, bool relative=true){
    if( id < 0 || id > 3) return;
    if(relative){
      if( pinched[id]){
        nav.pos().lerp( nav.pos() + getPinchTranslate(id), 0.01f);
      }else if( wasPinched[id]){
        //nav->move( ); //oldPos[id]);
      }
    }else{
      if( pinched[id]){
        nav.pos().lerp( pos[id], 0.1f);
      }else if( wasPinched[id]){
        //nav->move( ); //oldPos[id]);
      }
    }
  }
  
  // set nav velocity based on pinch translate vector when released
  void slingshotNavFromPinch( Nav *nav, int id){
    if( id < 0 || id > 3) return;
    if( pinched[id]){
      nav->pos().lerp(pos[id], 0.05f);
    }else if( wasPinched[id]){
      nav->move( -(pos[id] - pinchPos[id]) * Vec3f(1,1,-1));
    }
  }
  
};

// TODO
struct GlovePair {
  Glove left;
  Glove right;

  Vec3f getPinchScale(){
    Vec3f scale;
    return scale;
  }
  // Quatf getPinchRotation();
  
};

#endif
