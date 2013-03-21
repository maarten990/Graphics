/* Computer Graphics and Game Technology, Assignment Box2D game
 *
 * Student name ....
 * Student email ...
 * Collegekaart ....
 * Date ............
 * Comments ........
 *
 *
 * (always fill in these fields before submitting!!)
 */

#include <cstdio>

#include <GL/gl.h>
#include <GL/glut.h>
#include <Box2D/Box2D.h>

#include "levels.h"
#include "constants.h"
#include "GoalListener.h"

// prototypes
void createLevel(level_t &level);
void createBall(unsigned level);
void createGoal(unsigned int level);
void drawCircle(b2CircleShape *shape, b2Vec2 position);
void drawPolyShape(b2PolygonShape *shape);
void createPolygon(b2BodyType type, b2Vec2 *vertices, int amount);
float  calculateArea(b2Vec2 *vertices, int amount);
void drawWorld();

unsigned int reso_x = 800, reso_y = 600; // Window size in pixels
const float world_x = 8.f, world_y = 6.f; // Level (world) size in meters

int last_time;
int frame_count;

// Information about the levels loaded from files will be available in these.
unsigned int num_levels;
level_t *levels;

b2World *world;
unsigned g_level;

// Array that keeps track of the user created vertices
b2Vec2 *vertices_new = new b2Vec2[4];
int number;

// the collission callback object and corresponding variable
bool next_level = false;
GoalListener goallistener(&next_level);

// The ball body that gets attached to world
b2Body *ball;

/*
 * Load a given world, i.e. read the world from the `levels' data structure and
 * convert it into a Box2D world.
 */
void load_world(unsigned int level)
{
    if (level >= num_levels)
    {
        // Note that level is unsigned but we still use %d so -1 is shown as
        // such.
        printf("Warning: level %d does not exist.\n", level);
        return;
    }

    // Create the world and populate it with all bodies for this level
    // (including the ball).
    b2Vec2 gravity(0.0, -10);
    world = new b2World(gravity);
    world->SetContactListener(&goallistener);

    // create the level
    createBall(level);
    createGoal(level);
    createLevel( levels[level] );

    g_level = level;
}

// creates a ball in the world
void createBall(unsigned level)
{
    b2BodyDef ballDef;
    // definition
    ballDef.type = b2_staticBody;
    ballDef.position.Set(levels[level].start.x, levels[level].start.y);

    ball = world->CreateBody(&ballDef);
    ball->SetUserData(static_cast<int*>(&BALL));

    // shape
    b2CircleShape ballShape;
    ballShape.m_radius = 0.3;

    // fixture
    b2FixtureDef fixture;
    fixture.shape = &ballShape;
    //fixture.density = 1;
    //fixture.friction = 0.3;

    ball->CreateFixture(&fixture);

  
}

// Creates a polygon
void createPolygon(b2BodyType type, b2Vec2 *vertices, int amount)
{
    b2BodyDef bodyDef;

    // Add body to the world
    bodyDef.type = type;
    b2Body* body = world->CreateBody(&bodyDef);

    printf("\nArea: %f\n\n\n", calculateArea(vertices, amount));
    // create the shape and fixture
    b2PolygonShape polyShape;
    polyShape.Set(vertices, amount);

    b2FixtureDef fixtureDef;

    // Decide shape, density and friction
    fixtureDef.shape = &polyShape;
    fixtureDef.density = 1.0f;
    fixtureDef.friction = 0.3f;

    body->CreateFixture(&fixtureDef);
}

// create the goal
void createGoal(unsigned int level)
{
    b2BodyDef goaldef;
    goaldef.type = b2_staticBody;
    goaldef.position.Set(levels[level].end.x, levels[level].end.y);

    b2PolygonShape goalshape;
    goalshape.SetAsBox(0.05, 0.05);

    b2Body *goal = world->CreateBody(&goaldef);
    goal->CreateFixture(&goalshape, 0.0);
    goal->SetUserData(static_cast<void*>(&GOAL));
}

// Calculates the area of a polygon spanned with vertices
float calculateArea(b2Vec2 *vertices, int amount)
{
    float area = 0;
    for(int i = 0; i < (amount - 1) ; i++)
    {
        printf("points: %f, %f\n", vertices[i].x, vertices[i].y);
        area += ((vertices[i].x * vertices[i+1].y) - (vertices[i+1].x * vertices[i].y));
    }
    printf(" \n");
    return 0.5 * area;
}


// adds a given level to the global world as a series of static objects
void createLevel(level_t &level)
{
    poly_t poly;
    int num_verts;

    // loop through each polygon
    for (unsigned p = 0; p < level.num_polygons; ++p) {
        poly = level.polygons[p];
        num_verts = poly.num_verts;

        b2PolygonShape polyshape;
        b2Vec2 *vertices = new b2Vec2[num_verts];

        // loop through each vertex of the polyg0n
        for (int v = 0; v < num_verts; ++v) {
            vertices[v].Set(poly.verts[v].x,
                            poly.verts[v].y);
        }

        // create the new object
        createPolygon(b2_staticBody, vertices, num_verts);

        delete[] vertices;
    }
}



// draws a circle
void drawCircle(b2CircleShape *shape, b2Vec2 position)
{
    float radius = shape->m_radius;

    glColor3f(1.0f, 0.0f, 0.0f);
    glBegin(GL_TRIANGLE_FAN);

    // draw the center
    glVertex2f(position.x, position.y);
    
    for (float i = 0.0f; i <= (2 * M_PI) + 0.01; i += 0.1f) {
        glVertex2f(position.x + cos(i) * radius, position.y + sin(i) * radius);
    }

    glVertex2f(position.x + cos(0) * radius, position.y + sin(0) * radius);
    
    glEnd();
}

// draw a polygon shape
void drawPolyShape(b2PolygonShape *shape, b2Body *body)
{
    int vs = shape->GetVertexCount();
    b2Vec2 vertex;

    glBegin(GL_TRIANGLE_FAN);

    // draw the goal in blue, everything else in green
    if (body->GetUserData() &&
        *static_cast<int*>(body->GetUserData()) == GOAL) {
        glColor3f(0.0, 0.0, 1.0);
    } else {
        glColor3f(0.0, 1.0, 0.0);
    }


    for (int i = 0; i < vs; ++i) {
        vertex = body->GetWorldPoint(shape->GetVertex(i));
        glVertex2f(vertex.x, vertex.y);
    }

    glEnd();
}

void drawWorld()
{
    b2Body *body;
    b2Fixture *fixture;

    // loop through each body in the world, and each fixture in each body
    for (body = world->GetBodyList(); body; body = body->GetNext()) {
        for (fixture = body->GetFixtureList(); fixture; fixture = fixture->GetNext()) {
            // check if we're dealing with a ball or a polygonal object
            switch(fixture->GetType()) {
                case b2Shape::e_circle:
                    drawCircle( static_cast<b2CircleShape*>(fixture->GetShape()),
                                body->GetPosition());
                    break;
                case b2Shape::e_polygon:
                    drawPolyShape( static_cast<b2PolygonShape*>(fixture->GetShape()), body);
                    break;
                default:
                    // not implemented
                    break;
            }
        }
    }
}

/*
 * Called when we should redraw the scene (i.e. every frame).
 * It will show the current framerate in the window title.
 */
void draw(void)
{
    int time = glutGet(GLUT_ELAPSED_TIME);
    int frametime = time - last_time;
    frame_count++;

    // Clear the buffer
    glColor3f(0, 0, 0);
    glClear(GL_COLOR_BUFFER_BIT);


    //
    // Do any logic and drawing here.
    //
    world->Step(1.0f/255.0f, 8, 3);
    drawWorld();

    // check for a level transition
    if (next_level) {
        next_level = false;
        g_level += 1;
        load_world(g_level);
        return;
    }

    // Show rendered frame
    glutSwapBuffers();

    // Display fps in window title.
    if (frametime >= 1000)
    {
        char window_title[128];
        snprintf(window_title, 128,
                "Box2D: %f fps, level %d/%d",
                frame_count / (frametime / 1000.f), -1, num_levels);
        glutSetWindowTitle(window_title);
        last_time = time;
        frame_count = 0;
    }
}

/*
 * Called when window is resized. We inform OpenGL about this, and save this
 * for future reference.
 */
void resize_window(int width, int height)
{
    glViewport(0, 0, width, height);
    reso_x = width;
    reso_y = height;
}

/*
 * Called when the user presses a key.
 */
void key_pressed(unsigned char key, int x, int y)
{
    switch (key)
    {
        case 27: // Esc
        case 'q':
            exit(0);
            break;
        case 's':
            ball->SetType(b2_dynamicBody);
            break;
        // Add any keys you want to use, either for debugging or gameplay.
        default:
            break;
    }
}

/*
 * Called when the user clicked (or released) a mouse buttons inside the window.
 */
void mouse_clicked(int button, int state, int x, int y)
{
    // Only in case of click down
    if(state == GLUT_DOWN)
    {
        // add new position to vertices and increment
        vertices_new[number].Set(x / 100.0, (reso_y - y) /100.0);
        printf("added position: %f, %f\n", x /100.0, (reso_y - y) / 100.0);
        number += 1;

        // Create dynamic body in case of enough vertices (4)
        if(number == 4)
        {
            // Reset the number
            number = 0;

            // Create a dynamic object
            createPolygon(b2_dynamicBody, vertices_new, 4);
        }
    }


}

/*
 * Called when the mouse is moved to a certain given position.
 */
void mouse_moved(int x, int y)
{

}


int main(int argc, char **argv)
{
    // Create an OpenGL context and a GLUT window.
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
    glutInitWindowSize(reso_x, reso_y);
    glutCreateWindow("Box2D");

    // Bind all GLUT events do callback function.
    glutDisplayFunc(&draw);
    glutIdleFunc(&draw);
    glutReshapeFunc(&resize_window);
    glutKeyboardFunc(&key_pressed);
    glutMouseFunc(&mouse_clicked);
    glutMotionFunc(&mouse_moved);
    glutPassiveMotionFunc(&mouse_moved);

    // Initialise the matrices so we have an orthogonal world with the same size
    // as the levels, and no other transformations.
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0, world_x, 0, world_y, 0, 1);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    // Read the levels into a bunch of structs.
    num_levels = load_levels(&levels);
    printf("Loaded %d levels.\n", num_levels);

    // Load the first level (i.e. create all Box2D stuff).
    load_world(0);

    last_time = glutGet(GLUT_ELAPSED_TIME);
    frame_count = 0;
    glutMainLoop();

    return 0;
}
