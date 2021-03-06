#include <QMouseEvent>
#include <QGuiApplication>

#include "NGLScene.h"
#include <ngl/Random.h>
#include <ngl/ShaderLib.h>
#include <ngl/NGLInit.h>
#include <ngl/VAOPrimitives.h>

NGLScene::NGLScene(std::shared_ptr<Params> _p, std::shared_ptr<Time> _t)
{
    BoundingBox worldBB;
    worldBB.m_minx = -21.f;
    worldBB.m_maxx = 21.f;
    worldBB.m_miny = -21.f;
    worldBB.m_maxy = 21.f;
    m_world = std::make_unique<WorldGrid>(_p, 42, 42, worldBB, _t);
}

void NGLScene::resetScene()
{


}
NGLScene::~NGLScene()
{
  std::cout<<"Shutting down NGL, removing VAO's and Shaders\n";
}

void NGLScene::resizeGL( int _w, int _h )
{
  m_project=ngl::perspective( 45.0f, static_cast<float>( _w ) / _h, 0.05f, 350.0f );
  m_win.width  = static_cast<int>( _w * devicePixelRatio() );
  m_win.height = static_cast<int>( _h * devicePixelRatio() );
}

void NGLScene::initializeGL()
{
  // we must call this first before any other GL commands to load and link the
  // gl commands from the lib, if this is not done program will crash
  ngl::NGLInit::instance();
  m_transform.reset();

  glClearColor(0.4f, 0.4f, 0.4f, 1.0f);			   // Grey Background
  // enable depth testing for drawing
  glEnable(GL_DEPTH_TEST);
  // enable multisampling for smoother drawing
  glEnable(GL_MULTISAMPLE);
  // Now we will create a basic Camera from the graphics library
  // This is a static camera so it only needs to be set once
  // First create Values for the camera position
  ngl::Vec3 from(0.0f,80.0f,80.0f);
  ngl::Vec3 to(0.0f,0.0f,0.0f);
  ngl::Vec3 up(0.0f,1.0f,0.0f);
  m_view=ngl::lookAt(from,to,up);
  // set the shape using FOV 45 Aspect Ratio based on Width and Height
  // The final two are near and far clipping planes of 0.5 and 10
  m_project=ngl::perspective(45.0f,720.0f/576.0f,0.5f,150.0f);
  // now to load the shader and set the values
  // grab an instance of shader manager
   ngl::ShaderLib *shader=ngl::ShaderLib::instance();
   (*shader)["nglDiffuseShader"]->use();

   shader->setUniform("Colour",1.0f,1.0f,0.0f,1.0f);
   shader->setUniform("lightPos",1.0f,1.0f,1.0f);
   shader->setUniform("lightDiffuse",1.0f,1.0f,1.0f,1.0f);

   (*shader)["nglColourShader"]->use();
   shader->setUniform("Colour",1.0f,1.0f,1.0f,1.0f);

  glEnable(GL_DEPTH_TEST); // for removal of hidden surfaces

  ngl::VAOPrimitives *prim =  ngl::VAOPrimitives::instance();
  prim->createSphere("sphere",0.5f,40.0f);
  prim->createTorus("product",0.2,0.6,18,18);
  //prim->createSphere("product",0.3f,20.0f);
  //prim->loadBinary("cube", "./models/cube.obj", 0);

  //m_bbox->setDrawMode(GL_LINE);

  //m_sphereUpdateTimer=startTimer(40);

}


void NGLScene::loadMatricesToShader()
{
  ngl::ShaderLib *shader=ngl::ShaderLib::instance();
  (*shader)["nglDiffuseShader"]->use();

  ngl::Mat4 MV;
  ngl::Mat4 MVP;
  ngl::Mat3 normalMatrix;
  MV= m_view *m_mouseGlobalTX*m_transform.getMatrix();
  MVP=m_project *MV;
  normalMatrix=MV;
  normalMatrix.inverse().transpose();
  shader->setUniform("MVP",MVP);
  shader->setUniform("normalMatrix",normalMatrix);

  /*t.M=m_view*m_mouseGlobalTX*m_transform.getMatrix();

       t.MVP=m_project*t.M;
       t.normalMatrix=t.M;
       t.normalMatrix.inverse().transpose();
  shader->setUniformBuffer("TransformUBO",sizeof(transform),&t.MVP.m_00);*/
}

void NGLScene::loadMatricesToColourShader()
{
  ngl::ShaderLib *shader=ngl::ShaderLib::instance();
  (*shader)["nglColourShader"]->use();
  ngl::Mat4 MVP;
  MVP=m_project*m_view * m_mouseGlobalTX;
  shader->setUniform("MVP",MVP);

}


void NGLScene::paintGL()
{
  // clear the screen and depth buffer
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glViewport(0,0,m_win.width,m_win.height);
  // Rotation based on the mouse position for our global
   // transform
   ngl::Mat4 rotX;
   ngl::Mat4 rotY;
   // create the rotation matrices
   rotX.rotateX(m_win.spinXFace);
   rotY.rotateY(m_win.spinYFace);
   // multiply the rotations
   m_mouseGlobalTX=rotY*rotX;
   // add the translations
   m_mouseGlobalTX.m_m[3][0] = m_modelPos.m_x;
   m_mouseGlobalTX.m_m[3][1] = m_modelPos.m_y;
   m_mouseGlobalTX.m_m[3][2] = m_modelPos.m_z;

   updateScene();
   update();

   ngl::VAOPrimitives *prim=ngl::VAOPrimitives::instance();
    //prim->createSphere("sphere",0.5,40);

  // grab an instance of the shader manager
  ngl::ShaderLib *shader=ngl::ShaderLib::instance();
  (*shader)["nglColourShader"]->use();
  loadMatricesToColourShader();
  //m_box.draw();

  shader->use("nglDiffuseShader");

  std::vector<Agent*> ags = m_world.get()->getAgents();
  std::vector<GridCell*> bbs = m_world.get()->getWalls();
  std::vector<Vec2> prs = m_world.get()->getProducts();
  for(auto &a : ags)
  {
      m_transform.reset();
      {
          m_transform.setPosition(a->getPosition().x,0.0,a->getPosition().y);
          m_transform.setScale(a->getWeight(),a->getWeight(),a->getWeight());
          loadMatricesToShader();
          prim->draw("sphere");
      }
  }
  m_transform.reset();
  loadMatricesToShader();
  for(auto &p : prs)
  {
      m_transform.reset();
      {
          m_transform.setPosition(p.x,0.0,p.y);
          loadMatricesToShader();
          prim->draw("product");
          //std::cout<<p.x<<", "<<p.y<<std::endl;
      }
  }
  m_transform.reset();
  loadMatricesToShader();
  for(auto &b : bbs)
  {
      m_box.reset( new ngl::BBox(ngl::Vec3(b->m_position.x,0.0,b->m_position.y),b->m_limit.m_maxx-b->m_limit.m_minx,1.f,b->m_limit.m_maxy-b->m_limit.m_miny));
      m_box.get()->draw();
  }
}

//----------------------------------------------------------------------------------------------------------------------
void NGLScene::updateScene()
{
    m_world.get()->update();
}


//----------------------------------------------------------------------------------------------------------------------
void NGLScene::mouseMoveEvent( QMouseEvent* _event )
{
  // note the method buttons() is the button state when event was called
  // that is different from button() which is used to check which button was
  // pressed when the mousePress/Release event is generated
  if ( m_win.rotate && _event->buttons() == Qt::LeftButton )
  {
    int diffx = _event->x() - m_win.origX;
    int diffy = _event->y() - m_win.origY;
    m_win.spinXFace += static_cast<int>( 0.5f * diffy );
    m_win.spinYFace += static_cast<int>( 0.5f * diffx );
    m_win.origX = _event->x();
    m_win.origY = _event->y();
    update();
  }
  // right mouse translate code
  else if ( m_win.translate && _event->buttons() == Qt::RightButton )
  {
    int diffX      = static_cast<int>( _event->x() - m_win.origXPos );
    int diffY      = static_cast<int>( _event->y() - m_win.origYPos );
    m_win.origXPos = _event->x();
    m_win.origYPos = _event->y();
    m_modelPos.m_x += INCREMENT * diffX;
    m_modelPos.m_y -= INCREMENT * diffY;
    update();
  }
}


//----------------------------------------------------------------------------------------------------------------------
void NGLScene::mousePressEvent( QMouseEvent* _event )
{
  // that method is called when the mouse button is pressed in this case we
  // store the value where the maouse was clicked (x,y) and set the Rotate flag to true
  if ( _event->button() == Qt::LeftButton )
  {
    m_win.origX  = _event->x();
    m_win.origY  = _event->y();
    m_win.rotate = true;
  }
  // right mouse translate mode
  else if ( _event->button() == Qt::RightButton )
  {
    m_win.origXPos  = _event->x();
    m_win.origYPos  = _event->y();
    m_win.translate = true;
  }
}

//----------------------------------------------------------------------------------------------------------------------
void NGLScene::mouseReleaseEvent( QMouseEvent* _event )
{
  // that event is called when the mouse button is released
  // we then set Rotate to false
  if ( _event->button() == Qt::LeftButton )
  {
    m_win.rotate = false;
  }
  // right mouse translate mode
  if ( _event->button() == Qt::RightButton )
  {
    m_win.translate = false;
  }
}

//----------------------------------------------------------------------------------------------------------------------
void NGLScene::wheelEvent( QWheelEvent* _event )
{

  // check the diff of the wheel position (0 means no change)
  if ( _event->delta() > 0 )
  {
    m_modelPos.m_z += ZOOM;
  }
  else if ( _event->delta() < 0 )
  {
    m_modelPos.m_z -= ZOOM;
  }
  update();
}
//----------------------------------------------------------------------------------------------------------------------

void NGLScene::keyPressEvent(QKeyEvent *_event)
{
  // this method is called every time the main window recives a key event.
  // we then switch on the key value and set the camera in the GLWindow
  switch (_event->key())
  {
  // escape key to quite
  case Qt::Key_Escape : QGuiApplication::exit(EXIT_SUCCESS); break;
  // show full screen
  case Qt::Key_F : showFullScreen(); break;
  // show windowed
  case Qt::Key_N : showNormal(); break;
  case Qt::Key_R : resetScene(); break;

  default : break;
  }
  // finally update the GLWindow and re-draw
  //if (isExposed())
    update();
}

void NGLScene::timerEvent(QTimerEvent *_event )
{
    /*if(_event->timerId() == m_sphereUpdateTimer)
    {
        if (m_animate !=true)
        {
            return;
        }
    }*/
    //updateScene();
    update();
}


/*bool NGLScene::sphereSphereCollision( ngl::Vec3 _pos1, GLfloat _radius1, ngl::Vec3 _pos2, GLfloat _radius2 )
{
  // the relative position of the spheres
  ngl::Vec3 relPos;
  //min an max distances of the spheres
  GLfloat dist;
  GLfloat minDist;
  GLfloat len;
  relPos =_pos1-_pos2;
  // and the distance
  len=relPos.length();
  dist=len*len;
  minDist =_radius1+_radius2;
  // if it is a hit
  if(dist <=(minDist * minDist))
  {
    return true;
  }
  else
  {
    return false;
  }
}*/

//----------------------------------------------------------------------------------------------------------------------
/*void NGLScene::BBoxCollision()
{
  //create an array of the extents of the bounding box
  float ext[6];
  ext[0]=ext[1]=(m_bbox->height()/2.0f);
  ext[2]=ext[3]=(m_bbox->width()/2.0f);
  ext[4]=ext[5]=(m_bbox->depth()/2.0f);
  // Dot product needs a Vector so we convert The Point Temp into a Vector so we can
  // do a dot product on it
  ngl::Vec3 p;
  // D is the distance of the Agent from the Plane. If it is less than ext[i] then there is
  // no collision
  GLfloat D;
  // Loop for each sphere in the vector list
  for(Sphere &s : m_sphereArray)
  {
    p=s.getPos();
    //Now we need to check the Sphere agains all 6 planes of the BBOx
    //If a collision is found we change the dir of the Sphere then Break
    for(int i=0; i<6; ++i)
    {
      //to calculate the distance we take the dotporduct of the Plane Normal
      //with the new point P
      D=m_bbox->getNormalArray()[i].dot(p);
      //Now Add the Radius of the sphere to the offsett
      D+=s.getRadius();
      // If this is greater or equal to the BBox extent /2 then there is a collision
      //So we calculate the Spheres new direction
      if(D >=ext[i])
      {
        //We use the same calculation as in raytracing to determine the
        // the new direction
        GLfloat x= 2*( s.getDirection().dot((m_bbox->getNormalArray()[i])));
        ngl::Vec3 d =m_bbox->getNormalArray()[i]*x;
        s.setDirection(s.getDirection()-d);
        s.setHit();
      }//end of hit test
     }//end of each face test
    }//end of for
}*/

/*void  NGLScene::checkSphereCollisions()
{
  bool collide;

  unsigned int size=m_sphereArray.size();

    for(unsigned int ToCheck=0; ToCheck<size; ++ToCheck)
    {
        for(unsigned int Current=0; Current<size; ++Current)
        {
            // don't check against self
            if(ToCheck == Current)  continue;

      else
      {
        //cout <<"doing check"<<endl;
        collide =sphereSphereCollision(m_sphereArray[Current].getPos(),m_sphereArray[Current].getRadius(),
                                       m_sphereArray[ToCheck].getPos(),m_sphereArray[ToCheck].getRadius()
                                      );
        if(collide== true)
        {
          m_sphereArray[Current].reverse();
          m_sphereArray[Current].setHit();
        }
      }
    }
  }
}*/


/*void  NGLScene::checkCollisions()
{

    if(m_checkSphereSphere == true)
    {
        checkSphereCollisions();
    }
    BBoxCollision();

}*/

/*void NGLScene::removeSphere()
{
  std::vector<Sphere>::iterator end=m_sphereArray.end();
  if(--m_numSpheres==0)
  {
    m_numSpheres=1;
  }
  else
  {
    m_sphereArray.erase(end-1,end);
  }
}*/

/*void NGLScene::addSphere()
{
  ngl::Random *rng=ngl::Random::instance();
  ngl::Vec3 dir;
  dir=rng->getRandomVec3();
  // add the spheres to the end of the particle list
  m_sphereArray.push_back(Sphere(rng->getRandomPoint(s_extents,s_extents,s_extents),dir,rng->randomPositiveNumber(2)+0.5));
  ++m_numSpheres;
}*/




