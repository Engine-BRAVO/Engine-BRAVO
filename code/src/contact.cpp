#include "contact.h"

#include <iostream>

#include "box2d/box2d.h"
#include "box2d/id.h"
#include "box2d/types.h"

void Contact::getContact()
{
  for (int i = 0; i < contactlist.beginCount; i++)
  {
    std::cout << contactlist.beginEvents[i].shapeIdB.index1 << std::endl;
  }
}

void Contact::setContactList(b2WorldId worldID)
{
  contactlist = b2World_GetContactEvents(worldID);
}

void Contact::getBodyContact(b2BodyId body)
{
  b2ContactData contactData[10];
  int test = b2Body_GetContactData(body, contactData, 1);
  std::cout << test << std::endl;
}
