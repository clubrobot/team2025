//
// Created by boris on 5/21/25.
//

#include "Teleplot.h"



/// @brief Envoie le contenu du tampon de téléplot
void Teleplot::send_string(){
    printf("%s", this->tampon);
}

/// @brief Renvoi le temps en milliseconde, le temps réel ou le temps figé
/// @param
/// @return temps en millisecondes
unsigned long Teleplot::get_temps() const{
    if(this->temps_fige){
        return this->temps_ms;
    }
    return (long) (micros()/1000);
}

/// @brief Permet de "bloquer le temps" pour recevoir toutes les données datées à la même milliseconde
/// Simplifie beaucoup le traitement des données en CSV lors d'un import dans un tableur.
void Teleplot::fige_temps(){
    this->temps_fige = false;
    this->temps_ms = this->get_temps();
    this->temps_fige = true;
}
/// @brief Relâche le temps quand il est figé
/// Permet de reprendre le temps réel après un figement
void Teleplot::relache_temps(){
    this->temps_fige = false;
}

/// @brief Envoie le contenu du tampon de téléplot
/// Envoie le contenu du tampon de téléplot et vide le tampon
void Teleplot::envoie_tampon(){
    this->send_string();
    this->tampon[0]='\0'; // On "vide" le tampon
}

/// @brief Ajoute un message au tampon de téléplot
/// Si le tampon ne peut pas accueillir le prochain message, on envoie et on vide le tampon
void Teleplot::ajout_ou_envoie_tampon(const char * message){
    // Si le tampon ne peut pas accueillir le prochain message
    // On envoie et on vide le tampon
    if(strlen(message) + strlen(this->tampon) >= BEACON_MSG_LEN_MAX){
        this->envoie_tampon();
    }
    // On ajoute le message au tampon
    strcat(this->tampon, message);
}

/// @brief Ajoute une variable flottante au tampon de téléplot
void Teleplot::add_variable_float_2decimal(const char * nom_variable, const float valeur){
    char buf[INTERNAL_BUFFER_SIZE];
    snprintf(buf, INTERNAL_BUFFER_SIZE, ">%s:%lu:%.2f\n", nom_variable, this->get_temps(), valeur);
    this->ajout_ou_envoie_tampon(buf);
}

/// @brief Ajoute une variable entière au tampon de téléplot
void Teleplot::add_variable_int(const char * nom_variable, const int valeur){
    char buf[INTERNAL_BUFFER_SIZE];
    snprintf(buf, INTERNAL_BUFFER_SIZE, ">%s:%lu:%d\n", nom_variable, this->get_temps(), valeur);
    this->ajout_ou_envoie_tampon(buf);
}

/// @brief Ajoute une variable 2D au tampon de téléplot
void Teleplot::add_variable_2d(const char * nom_variable, const float x, const float y){
    char buf[INTERNAL_BUFFER_SIZE];
    snprintf(buf, INTERNAL_BUFFER_SIZE, ">%s:%.2f:%.2f:%lu|xy\n", nom_variable, x, y, this->get_temps());
    this->ajout_ou_envoie_tampon(buf);
}

/// @brief Ajoute un status au tampon de téléplot
 void Teleplot::add_status(const char* nom_variable, const char* status) {
    char buf[INTERNAL_BUFFER_SIZE];
    snprintf(buf,INTERNAL_BUFFER_SIZE, ">%s:%lu:%s|t\n", nom_variable, this->get_temps(), status);
    this->ajout_ou_envoie_tampon(buf);
}

/// @brief Ajoute un cube au tampon de téléplot
/// @param param Paramètres du cube à ajouter
/// @note Les paramètres du cube sont dans l'ordre : nom, position (x, y, z), taille (width, height, depth), rotation (quat_x, quat_y, quat_z, quat_w), couleur et transparence (optionnel).
/// @note On peut réutiliser la dernière valeur en ne remplissant que les paramètres modifiés
void Teleplot::add_cube(cube_parameters param) {
    char buf[INTERNAL_BUFFER_SIZE];
    snprintf(buf, INTERNAL_BUFFER_SIZE, ">3d|%s:%lu:S:cube:P:%s:%s:%s:Q:%s:%s:%s:%s:W:%s:H:%s:D:%s:C:%s:O:%s\n",
        param.nom_forme,
        this->get_temps(),
        param.pos_x,
        param.pos_y,
        param.pos_z,
        param.quat_x,
        param.quat_y,
        param.quat_z,
        param.quat_w,
        param.width,
        param.height,
        param.depth,
        param.color,
        param.transparency
    );
    this->ajout_ou_envoie_tampon(buf);

}

/// @brief Ajoute une sphère au tampon de téléplot
/// @param param Paramètres de la sphère à ajouter
/// @note Les paramètres de la sphère sont dans l'ordre : nom, position (x, y, z), rayon, précision (nombre de rectangles), rotation (quat_x, quat_y, quat_z, quat_w), couleur et transparence (optionnel).
/// @note On peut réutiliser la dernière valeur en ne remplissant que les paramètres modifiés
void Teleplot::add_sphere(sphere_parameters param) {
    char buf[INTERNAL_BUFFER_SIZE];
    snprintf(buf, INTERNAL_BUFFER_SIZE, ">3d|%s:%lu:S:sphere:P:%s:%s:%s:RA:%s:PR:%s:Q:%s:%s:%s:%s:C:%s:O:%s\n",
        param.nom_forme,
        this->get_temps(),
        param.pos_x,
        param.pos_y,
        param.pos_z,
        param.radius,
        param.precision,
        param.quat_x,
        param.quat_y,
        param.quat_z,
        param.quat_w,
        param.color,
        param.transparency
    );
    this->ajout_ou_envoie_tampon(buf);
}

/// @brief Ajoute un message de log au tampon de téléplot
/// @param message Le message à ajouter au tampon
void Teleplot::add_log(const char *message) {
    char buf[INTERNAL_BUFFER_SIZE];
    snprintf(buf, INTERNAL_BUFFER_SIZE, ">%lu:%s\n", this->get_temps(), message);
    this->ajout_ou_envoie_tampon(buf);
}

