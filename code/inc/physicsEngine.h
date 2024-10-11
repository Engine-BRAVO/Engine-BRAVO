struct gravity
{
  float x;
  float y;
};

class PhysicsEngine
{
 public:
  PhysicsEngine();
  void gameplayLoop();
  void updatePhysics();
  void createWorld(gravity gravity);

 private:
};
