#include "Sphere.h"
#include <ngl/VAOPrimitives.h>


Sphere::Sphere( ngl::Vec3 _pos, ngl::Vec3 _dir,  GLfloat _rad)
{
  // set values from params
  m_pos=_pos;
  m_dir=_dir;
  m_radius=_rad;
  m_hit=false;
}

Sphere::Sphere()
{
  m_hit=false;
}

void Sphere::loadMatricesToShader( ngl::Transformation &_tx, const ngl::Mat4 &_globalMat,const  ngl::Mat4 &_view,const ngl::Mat4 &_project  ) const
{
  ngl::ShaderLib *shader=ngl::ShaderLib::instance();

  ngl::Mat4 MV;
  ngl::Mat4 MVP;
  ngl::Mat3 normalMatrix;
  MV=_view  *_globalMat* _tx.getMatrix();
  MVP=_project*MV;
  normalMatrix=MV;
  normalMatrix.inverse().transpose();
  shader->setUniform("MVP",MVP);
  shader->setUniform("normalMatrix",normalMatrix);
}


void Sphere::draw(const std::string &_shaderName, const ngl::Mat4 &_globalMat,  const ngl::Mat4 &_view , const ngl::Mat4 &_project)const
{

  // draw wireframe if hit
  if(m_hit)
  {
    glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);
  }
  else
  {
    glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);
  }


  ngl::ShaderLib *shader=ngl::ShaderLib::instance();
  shader->use(_shaderName);
  // grab an instance of the primitives for drawing
  ngl::VAOPrimitives *prim=ngl::VAOPrimitives::instance();
  ngl::Transformation t;

  t.setPosition(m_pos);
  t.setScale(m_radius,m_radius,m_radius);
  loadMatricesToShader(t,_globalMat,_view,_project);
  prim->draw("sphere");

glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);
}



void Sphere :: set(ngl::Vec3 _pos, ngl::Vec3 _dir, GLfloat _rad)
{
  m_pos=_pos;
  m_dir=_dir;
  m_radius=_rad;
}

void Sphere::move()
{
  // store the last position
  m_lastPos=m_pos;
  // update the current position
  m_pos+=m_dir;
  // get the next position
  m_nextPos=m_pos+m_dir;
  m_hit=false;
}












