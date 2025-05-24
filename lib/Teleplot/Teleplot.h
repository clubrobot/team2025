// Teleplot
// Source: https://github.com/nesnes/teleplot

#ifndef TELEPLOT_H
#define TELEPLOT_H

#include <Arduino.h>
#include <Logger.h>

#define BEACON_MSG_LEN_MAX 256
#define INTERNAL_BUFFER_SIZE 200

typedef struct cube_parameters{
    const char* nom_forme;
    // position
    const char * pos_x;
    const char * pos_y;
    const char * pos_z;
    // taille
    const char * width;
    const char * height;
    const char * depth;
    // rotation (quaternion)
    const char * quat_x;
    const char * quat_y;
    const char * quat_z;
    const char * quat_w;
    //color
    const char* color;
    // transparency
    const char * transparency;
} cube_parameters;

typedef struct sphere_parameters{
    const char* nom_forme;
    // position
    const char * pos_x;
    const char * pos_y;
    const char * pos_z;

    const char * radius;

    // rotation (quaternion)
    const char * quat_x;
    const char * quat_y;
    const char * quat_z;
    const char * quat_w;

    const char * precision; // precision of the sphere, how many rectangle to draw it
    // color
    const char* color;
    // transparency
    const char * transparency;
} sphere_parameters;

class Teleplot {
private:
    char tampon[BEACON_MSG_LEN_MAX]="";

    long temps_ms;
    bool temps_fige;
public:
    void send_string();

    unsigned long get_temps() const;

    void fige_temps();

    void relache_temps();

    void envoie_tampon();

    void ajout_ou_envoie_tampon(const char *message);

    void add_variable_float_2decimal(const char *nom_variable, const float valeur);

    void add_variable_int(const char *nom_variable, const int valeur);

    void add_variable_2d(const char *nom_variable, const float x, const float y);

    void add_status(const char* nom_variable, const char* status);

    void add_cube(cube_parameters param);

    void add_sphere(sphere_parameters param);

    void add_log(const char *message);
};

#endif
