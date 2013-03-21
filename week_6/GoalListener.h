#ifndef GOAL_LISENER_H
#define GOAL_LISENER_H

#include <Box2D/Box2D.h>
#include "constants.h"

/*
 * Contactlistener for detecting a collission between ball and goal
 */
class GoalListener : public b2ContactListener
{
    private:
    bool *m_level_complete;

    public:
    GoalListener(bool *level_complete)
    {
        m_level_complete = level_complete;
    }

    void BeginContact(b2Contact* contact)
    {
        b2Body *b1 = contact->GetFixtureA()->GetBody(),
               *b2 = contact->GetFixtureB()->GetBody();

        // make sure there is userdata on both bodies
        if (!b1->GetUserData() || !b2->GetUserData()) {
            return;
        }

        // if the collission is between a ball and a goal, set the passed
        // variable to true
        int id1 = *static_cast<int*>(b1->GetUserData()),
            id2 = *static_cast<int*>(b2->GetUserData());

        if ( (id1 == GOAL && id2 == BALL) ||
             (id1 == BALL && id2 == GOAL) )
        {
            *m_level_complete = true;
        };
    }
};

#endif
