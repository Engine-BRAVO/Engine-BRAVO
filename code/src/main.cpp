#include "Enemy.h"
#include "dummy_nodes.h"

using namespace BT;

/** Behavior Tree are used to create a logic to decide what
 * to "do" and when. For this reason, our main building blocks are
 * Actions and Conditions.
 *
 * In this tutorial, we will learn how to create custom ActionNodes.
 * It is important to remember that NodeTree are just a way to
 * invoke callbacks (called tick() ). These callbacks are implemented by the
 * user.
 */

// clang-format off
static const char* xml_text = R"(

 <root BTCPP_format="4" >

     <BehaviorTree ID="MainTree">
        <Sequence name="root_sequence">
            <CheckBattery   name="battery_ok"/>
            <OpenGripper    name="open_gripper"/>
            <ApproachObject name="approach_object"/>
            <CloseGripper   name="close_gripper"/>
        </Sequence>
     </BehaviorTree>

 </root>
 )";

// clang-format on

void example() {
  /* Expected output:
  *
         [ Battery: OK ]
         GripperInterface::open
         ApproachObject: approach_object
         GripperInterface::close
  */
  // We use the BehaviorTreeFactory to register our custom nodes
  BehaviorTreeFactory factory;

  // Note: the name used to register should be the same used in the XML.
  // Note that the same operations could be done using
  // DummyNodes::RegisterNodes(factory)

  using namespace DummyNodes;

  // The recommended way to create a Node is through inheritance.
  // Even if it requires more boilerplate, it allows you to use more
  // functionalities like ports (we will discuss this in future tutorials).
  factory.registerNodeType<ApproachObject>("ApproachObject");

  // Registering a SimpleActionNode using a function pointer.
  // you may also use C++11 lambdas instead of std::bind
  factory.registerSimpleCondition("CheckBattery",
                                  [&](TreeNode &) { return CheckBattery(); });

  // You can also create SimpleActionNodes using methods of a class
  GripperInterface gripper;
  factory.registerSimpleAction("OpenGripper",
                               [&](TreeNode &) { return gripper.open(); });
  factory.registerSimpleAction("CloseGripper",
                               [&](TreeNode &) { return gripper.close(); });

  // Trees are created at deployment-time (i.e. at run-time, but only once at
  // the beginning). The currently supported format is XML. IMPORTANT: when the
  // object "tree" goes out of scope, all the TreeNodes are destroyed
  auto tree = factory.createTreeFromText(xml_text);

  // To "execute" a Tree you need to "tick" it.
  // The tick is propagated to the children based on the logic of the tree.
  // In this case, the entire sequence is executed, because all the children
  // of the Sequence return SUCCESS.
  tree.tickWhileRunning();
}

BT::NodeStatus MoveAction(Enemy &enemy) {
  enemy.move();
  return BT::NodeStatus::SUCCESS;
}

BT::NodeStatus AttackAction(Enemy &enemy) {
  enemy.attack();
  return BT::NodeStatus::SUCCESS;
}

BT::NodeStatus StealAction(Enemy &enemy) {
  enemy.steal();
  return BT::NodeStatus::SUCCESS;
}

void customTest() {
  static const char *customXml = R"(

 <root BTCPP_format="4" >

     <BehaviorTree ID="MainTree">
        <Sequence name="root_sequence">
            <Move   name="Move"/>
            <Attack    name="Attack"/>
            <Steal name="Steal"/>
        </Sequence>
     </BehaviorTree>

 </root>
 )";
  Enemy enemy;
  BehaviorTreeFactory factory;

  // factory.registerSimpleAction("Move",
  //                              [&enemy]() { return MoveAction(enemy); });
  factory.registerSimpleAction("Move", std::bind(MoveAction, enemy));
  factory.registerSimpleAction(
      "Attack", [&enemy](TreeNode &) { return AttackAction(enemy); });
  factory.registerSimpleAction(
      "Steal", [&enemy](TreeNode &) { return StealAction(enemy); });

  auto tree = factory.createTreeFromText(customXml);

  tree.tickWhileRunning();
}

int main() { customTest(); }