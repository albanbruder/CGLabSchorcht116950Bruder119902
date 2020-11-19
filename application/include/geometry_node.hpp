#ifndef NODE_HPP
#define NODE_HPP

class GeometryNode: public Node {
  public:
    model getGeometry();
    void setGeometry(model newGeometry);
  
  private:
    model geometry;
};

#endif