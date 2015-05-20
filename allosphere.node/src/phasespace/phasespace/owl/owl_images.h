// owl_images.h
// OWL v1.3

#ifndef OWL_IMAGES_H
#define OWL_IMAGES_H

#ifdef __cplusplus
extern "C" {
#endif

struct OWLImage {
  int id;
  int frame;
  int camera;
  int detector;
  int width;
  uint_t flag;
  
  float pos;
  float amp;
};

struct OWLDetectors {
  int id;
  int frame;
  uint_t detectors[8];
};

OWLAPI int owlGetImages(OWLImage *images, uint_t count);
OWLAPI int owlGetDetectors(OWLDetectors *detectors, uint_t count);

#ifdef __cplusplus
}
#endif

#endif // OWL_IMAGES_H
