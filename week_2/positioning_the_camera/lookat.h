/* Computer Graphics, Assignment 4, Positioning the camera
 *
 * Filename ........ lookat.h
 * Description ..... Contains the re-programmed lookAt function
 * Created by ...... Jurgen Sturm 
 */


#ifndef IBS_LOOKAT_H
#define IBS_LOOKAT_H


void myLookAt(GLdouble eyeX, GLdouble eyeY, GLdouble eyeZ,
              GLdouble centerX, GLdouble centerY, GLdouble centerZ,
              GLdouble upX, GLdouble upY, GLdouble upZ);
void normalize(GLdouble *v);
void cross(GLdouble *target, GLdouble *x, GLdouble *y);


#endif /* IBS_LOOKAT_H */
