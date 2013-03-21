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

unsigned int reso_x = 800, reso_y = 600; // Window size in pixels
const float world_x = 8.f, world_y = 6.f; // Level (world) size in meters

int last_time;
int frame_count;

// Information about the levels loaded from files will be available in these.
unsigned int num_levels;
level_t *levels;

// Defining world; only needs to be done once
b2Vec2 gravity(0.0f, -3.0f);
b2World world(gravity);

// Values for step increment
float32 timeStep = 1.0f / 60.0f;
int32 velocityIterations = 6;
int32 positionIterations = 2;


// Array that keeps track of the user created vertices
b2Vec2 *vertices_new = new b2Vec2[4];
int number;

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
    // Create a Box2D world and populate it with all bodies for this level
    // (including the ball).
    level_t l = levels[2];

    // Create ball and put at the beginning position
    b2Body* ball;
    b2BodyDef bodyDef;
    bodyDef.type = b2_dynamicBody;
    bodyDef.position.Set(l.start.x, l.start.y);
    ball = world.CreateBody(&bodyDef);
    b2CircleShape ballShape;
    ballShape.m_radius = 0.2;
    ball->CreateFixture(&ballShape, 0.0f);


    // The body and shape for the general poly builds
    poly_t p;
    b2PolygonShape pol_shape;
    b2BodyDef pol_body_def;
    b2Body*  pol_body;

    // Loop through all polygons
    for(unsigned int i = 0; i < l.num_polygons; i ++)
    {
        p = l.polygons[i];
        b2Vec2 *vertices = new b2Vec2[p.num_verts];

        // Set all vertices over into b2Vec2 vertices
        for(unsigned int j = 0; j < p.num_verts; j++ )
        {
            vertices[j].Set(p.verts[j].x, p.verts[j].y);
        }

        // Add body_def to the world object
        pol_body  = world.CreateBody(&pol_body_def);

        // Create the fixture and set vertices
        pol_shape.Set(vertices, p.num_verts);
        pol_body->CreateFixture(&pol_shape, 0.0f);
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

    world.Step(timeStep, velocityIterations, positionIterations);

    // Clear the buffer
    glColor3f(0, 0, 0);
    glClear(GL_COLOR_BUFFER_BIT);

    // Get a list of all the bodies
    b2Body* body = world.GetBodyList();

    // Some predeclared variables for the loop
    b2Fixture* types;
    b2Vec2 position;

    // While there are still new bodies
    while(true)
    {    
        types = body->GetFixtureList();
        position = body->GetPosition();

        while(true)
        {

            // Draw circle
            if(types->GetType() == b2Shape::e_circle)
            {
                // Cast to subclass shape
                b2CircleShape* bal_shape = static_cast<b2CircleShape*>(types->GetShape());
                glColor3f(1, 0, 0);

                //draw vertices
                glBegin(GL_TRIANGLE_FAN);
                glVertex2f(position.x, position.y);
                glVertex2f(position.x, position.y + bal_shape->m_radius);
                glVertex2f(position.x + bal_shape->m_radius, position.y + bal_shape->m_radius);
                glVertex2f(position.x + bal_shape->m_radius, position.y - bal_shape->m_radius);
                glVertex2f(position.x - bal_shape->m_radius , position.y - bal_shape->m_radius);
                glEnd();
            }
            // Draw polygon
            else
            {
                // Cast to subclass shape
                b2PolygonShape* pol_shape = static_cast<b2PolygonShape*>(types->GetShape());

                //printf("position x y: %f %f\n", position.x, position.y);
    
                glColor3f(0, 1, 0);
                glBegin(GL_TRIANGLE_FAN);

                // Draw all the vertices until there are no new ones
                for(int i = 0; i < pol_shape->GetVertexCount(); i++)
                {
                    b2Vec2 vpos = body->GetWorldPoint(pol_shape->GetVertex(i));
                    glVertex2f(vpos.x, vpos.y);
                }
                glEnd();

            }

            // Break if there are no more fixtures
            types = types->GetNext();
            if(types == NULL)
            {
                break;
            }
        }

        // Get the next body in the linked list
        body = body->GetNext();

        // Break if there are no new bodies
        if(body == NULL)
        {
            break;
        }

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

            // Add any keys you want to use, either for debugging or gameplay.
        default:
            break;
    }
}


// Calculates the area of a given polygon
double calculate_area()
{
    double area = 0;
    for(int i = 0 ; i < 3; i ++)
    {
        area += (vertices_new[i].x * vertices_new[i+1].y) - 
            (vertices_new[i+1].x * vertices_new[i].y);

    }

    return 0.5 * area;
}
// Reverse the order of the elements
b2Vec2* swap_elements(b2Vec2* vertices, int num_vertices)
{
    b2Vec2 *temp = new b2Vec2[num_vertices];
    // Swap last elements to first elements
    for(int i = 0; i < num_vertices; i++)
    {
        temp[i].x = vertices[num_vertices - i].x;
        temp[i].y = vertices[num_vertices - i].y;
    }
    return temp;
}


void create_dynamic_object()
{

    b2BodyDef bodyDef;

    //bodyDef.position.Set(0.0f, 4.0f);
    
    // Add body to the world
    bodyDef.type = b2_dynamicBody;
    b2Body* body = world.CreateBody(&bodyDef);


    printf(" calculate area: %f", calculate_area());
    //if(calculate_area() < 0){
     //   vertices_new = swap_elements(vertices_new, 4);
    //};

    // Set shape and vertices
    b2PolygonShape polyShape;
    polyShape.Set(vertices_new, 4);

    b2FixtureDef fixtureDef;

    // Decide shape, density and friction
    fixtureDef.shape = &polyShape;
    fixtureDef.density = 1.0f;
    fixtureDef.friction = 0.3f;

    body->CreateFixture(&fixtureDef);
}

/*
 * Called when the user clicked (or released) a mouse buttons inside the window.
 */
void mouse_clicked(int button, int state, int x, int y)
{
    if(state == GLUT_DOWN)
    {

        printf("Mouse clicked\n");
        printf("x, y: %f, %f\n", x / 100.0, y / 100.0);
        vertices_new[number].Set(x / 100.0, (reso_y - y) /100.0);
        number += 1;

        if(number == 4)
        {
            // Reset the number
            number = 0;

            create_dynamic_object();
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
