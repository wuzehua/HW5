#ifndef _MATERIAL_H_
#define _MATERIAL_H_

#include "vectors.h"
#include "ray.h"
#include "hit.h"

// ====================================================================
// ====================================================================



// You will extend this class in later assignments

class Material {

public:

  // CONSTRUCTORS & DESTRUCTOR
  Material(const Vec3f &d_color, const Vec3f& s_color, const Vec3f& r_color, const Vec3f& t_color, const float& index)
  {
    diffuseColor = d_color;
    specularColor = s_color;
    reflectiveColor = r_color;
    transparentColor = t_color;
    indexOfRefraction = index;
  }
  virtual ~Material() {}

  virtual void glSetMaterial(void) const = 0;

  // ACCESSORS
  virtual Vec3f getDiffuseColor() const { return diffuseColor; }
  virtual Vec3f getSpecularColor() const{ return specularColor;}
  virtual Vec3f getReflectiveColor() const{ return reflectiveColor;}
  virtual Vec3f getTransparentColor() const{ return transparentColor;}
  virtual float getIndexOfRefraction() const{ return indexOfRefraction;}

  virtual Vec3f Shade(const Ray &ray, const Hit &hit, const Vec3f &dirToLight,const Vec3f &lightColor) const = 0;

protected:

  // REPRESENTATION
  Vec3f diffuseColor;
  Vec3f specularColor;
  Vec3f reflectiveColor;
  Vec3f transparentColor;
  float indexOfRefraction;
  
};



// ====================================================================
// ====================================================================

#endif
