
#ifndef DEBUGGRAPHICS_H
#define DEBUGGRAPHICS_H

#include "allocore/graphics/al_Graphics.hpp"
#include "Glove.hpp"

struct DebugDraw {

  static void drawGlove(Graphics& g, Glove& glove){
    g.pointSize(12.f);
    g.lineWidth(2.f);

    Vec3f color(0,0,1);

    //finger / pinch points and vectors
    for(int i=0; i < 4; i++){
      if( glove.pinched[i] ){
        g.color(0.f,1.f,0.f);
        g.begin(Graphics::POINTS);
        g.vertex(glove.pos[i]);
        g.pointSize(10.f);
        g.vertex(glove.pinchPos[i]);
        g.end();
        g.pointSize(15.f);
        g.begin(Graphics::LINES);
        g.vertex(glove.pos[i]);
        g.vertex(glove.pinchPos[i]);
        g.end();

      } else if( glove.seen[i]){
        g.color(color);
        g.begin(Graphics::POINTS);
        g.vertex(glove.pos[i]);
        g.end();
        
        g.lineWidth(6.f);
        if(i<2 && glove.seen[eBackPinkySide]){
          g.begin(Graphics::LINES);
          g.vertex(glove.pos[i]);
          g.vertex(glove.pos[eBackPinkySide]);
          g.end();
        }else if( glove.seen[eBackThumbSide]){
          g.begin(Graphics::LINES);
          g.vertex(glove.pos[i]);
          g.vertex(glove.pos[eBackThumbSide]);
          g.end();  
        }
      }

    }

    // rest of glove points
    for(int i=4; i<8; i++){
      if( !glove.seen[i] ) continue;
      g.color(color);
      g.begin(Graphics::POINTS);
      g.vertex(glove.pos[i]);
      g.end();
    }

    // thumb and back of hand vectors
    g.color(color);
    if( glove.seen[eThumbTip] && glove.seen[eThumbBase]){
      g.begin(Graphics::LINES);
      g.vertex(glove.pos[eThumbTip]);
      g.vertex(glove.pos[eThumbBase]);
      g.end();
    }
    if( glove.seen[eBackThumbSide] && glove.seen[eBackPinkySide]){
      g.begin(Graphics::LINES);
      g.vertex(glove.pos[eBackThumbSide]);
      g.vertex(glove.pos[eBackPinkySide]);
      g.end();
    }

    // centroid
    g.color(1.f,0.f,0.f);
    g.begin(Graphics::POINTS);
    g.vertex(glove.centroid);
    g.end();
    g.lineWidth(1.f);
  }
  
};

#endif