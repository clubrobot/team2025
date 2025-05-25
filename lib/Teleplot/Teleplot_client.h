// Teleplot
// Source: https://github.com/nesnes/teleplot

#ifndef TELEPLOT_H
#define TELEPLOT_H

#include <Arduino.h>
#include <Logger.h>

#define BEACON_MSG_LEN_MAX 256
#define INTERNAL_BUFFER_SIZE 200

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

    void add_log(const char *message);
};

#endif
