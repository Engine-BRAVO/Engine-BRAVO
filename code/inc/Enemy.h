#include "behaviortree_cpp/behavior_tree.h"

class Enemy {
public:
  void move();

  void attack();

  void steal();
};

// Wrapper functions that interact with the enemy object
// BT::NodeStatus MoveAction(Enemy &enemy);

// BT::NodeStatus AttackAction(Enemy &enemy);

// BT::NodeStatus StealAction(Enemy &enemy);