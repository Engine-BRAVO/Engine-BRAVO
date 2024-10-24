#include "contact.h"

#include <iostream>

#include "box2d/box2d.h"

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
