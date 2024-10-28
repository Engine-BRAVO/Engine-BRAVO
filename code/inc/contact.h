#include "box2d/box2d.h"
#include "box2d/id.h"
#include "box2d/types.h"

class Contact : public b2ContactEvents
{
 public:
  void getContact();
  void setContactList(b2WorldId worldID);
  void getBodyContact(b2BodyId);

 private:
  b2ContactEvents contactlist;
};
