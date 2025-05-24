// Teleplot
// Source: https://github.com/nesnes/teleplot

#ifndef TELEPLOT_H
#define TELEPLOT_H

#include <Arduino.h>
#include <Logger.h>

#define BEACON_MSG_LEN_MAX 256

typedef struct cube_parameters{
    char* nom_forme;
    // position
    char * pos_x;
    char * pos_y;
    char * pos_z;
    // taille
    char * width;
    char * height;
    char * depth;
    // rotation (quaternion)
    char * quat_x;
    char * quat_y;
    char * quat_z;
    char * quat_w;
    //color
    char* color;
    // transparency
    char * transparency;
} cube_parameters;

typedef struct sphere_parameters{
    char* nom_forme;
    // position
    char * pos_x;
    char * pos_y;
    char * pos_z;

    char * radius;

    // rotation (quaternion)
    char * quat_x;
    char * quat_y;
    char * quat_z;
    char * quat_w;

    char * precision; // precision of the sphere, how many rectangle to draw it
    // color
    char* color;
    // transparency
    char * transparency;
} sphere_parameters;

class Teleplot {
private:
    char tampon[BEACON_MSG_LEN_MAX]="";

    long temps_ms;
    bool temps_fige;
public:
    void send_string();

    long get_temps(void) const;

    void fige_temps(void);

    void relache_temps(void);

    void envoie_tampon(void);

    void ajout_ou_envoie_tampon(char *message);

    void add_variable_float_2decimal(const char *nom_variable, float valeur);

    void add_variable_int(char *nom_variable, int valeur);

    void add_variable_2d(char *nom_variable, float x, float y);

    void add_status(char* nom_variable, char* status);

    void add_cube(cube_parameters param);

    void add_sphere(sphere_parameters param);

    void add_log(const char *message);
};

#endif
