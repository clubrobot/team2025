//
// Created by awing on 25/05/25.
//
#include "Teleplot_client.h"

#ifndef TEAM2025_SPHERE_H
#define TEAM2025_SPHERE_H


class Sphere {
public:
    /**
     * @Brief Créé une sphère pour le téléplot
     *
     * @param nom_forme
     * @param pos_x
     * @param pos_y
     * @param pos_z
     * @param radius
     * @param quat_x
     * @param quat_y
     * @param quat_z
     * @param quat_w
     * @param precision
     * @param color
     * @param transparency
     */
    Sphere(char* nom_forme, float pos_x, float pos_y, float pos_z,
           float radius, float quat_x, float quat_y, float quat_z, float quat_w,
           int precision, char* color, float transparency);
    /**
     * @brief modifie la position de la sphère
     *
     * @param pos_x
     * @param pos_y
     * @param pos_z
     */
    void setPosition(float pos_x, float pos_y, float pos_z);

    /**
     * @brief modifie la position x de la sphère
     *
     * @param pos_x
     */
    void setXPosition(float pos_x);

    /**
     * @brief modifie la position y de la sphère
     *
     * @param pos_y
     */
    void setYPosition(float pos_y);

    /**
     * @brief modifie la position z de la sphère
     *
     * @param pos_z
     */
    void setZPosition(float pos_z);

    /**
     * @brief modifie le rayon de la sphère
     *
     * @param radius
     */
    void setRadius(float radius);

    /**
     * @brief modifie la précision de la sphère
     *
     * @param precision
     */
    void setPrecision(int precision);

    /**
     * @brief modifie le quaternion de rotation de la sphère
     *
     * @param quat_x
     * @param quat_y
     * @param quat_z
     * @param quat_w
     */
    void setRotation(float quat_x, float quat_y, float quat_z, float quat_w);

    /**
     * @brief modifie le quaternion de rotation x de la sphère
     *
     * @param quat_x
     */
    void setQuatX(float quat_x);

    /**
     * @brief modifie le quaternion de rotation y de la sphère
     *
     * @param quat_y
     */
    void setQuatY(float quat_y);

    /**
     * @brief modifie le quaternion de rotation z de la sphère
     *
     * @param quat_z
     */
    void setQuatZ(float quat_z);

    /**
     * @brief modifie le quaternion de rotation w de la sphère
     *
     * @param quat_w
     */
    void setQuatW(float quat_w);

    /**
     * @brief modifie la couleur de la sphère
     *
     * @param color
     */
    void setColor(const char* color);

    /**
     * @brief modifie la transparence de la sphère
     *
     * @param transparency
     */
    void setTransparency(float transparency);

    /**
     * @brief Envoie la sphère à Teleplot
     *
     * @param teleplot L'instance de Teleplot à laquelle les données de la sphère seront envoyées.
     */
    void sendSphereToTeleplot(Teleplot &teleplot);

private:
    static const char NAME_LENGTH = 50; // Length of the shape name
    static const char NUMBER_PRECISION = 20; // Precision for floating point numbers

    const short MASK_NAME =         0b0000000000000001;
    const short MASK_POS_X =        0b0000000000000010;
    const short MASK_POS_Y =        0b0000000000000100;
    const short MASK_POS_Z =        0b0000000000001000;
    const short MASK_RADIUS =       0b0000000000010000;
    const short MASK_PRECISION =    0b0000000000100000;
    const short MASK_QUAT_X =       0b0000000010000000;
    const short MASK_QUAT_Y =       0b0000000100000000;
    const short MASK_QUAT_Z =       0b0000001000000000;
    const short MASK_QUAT_W =       0b0000010000000000;
    const short MASK_COLOR =        0b0000100000000000;
    const short MASK_TRANSPARENCY = 0b0001000000000000;

    char nom_forme[NAME_LENGTH]; // Name of the sphere, initialized to an empty string
    // position
    float pos_x;
    float pos_y;
    float pos_z;
    char r_pos_x[NUMBER_PRECISION];
    char r_pos_y[NUMBER_PRECISION];
    char r_pos_z[NUMBER_PRECISION];
    // radius
    float radius;
    char r_radius[NUMBER_PRECISION];
    // precision
    int precision;
    char r_precision[NUMBER_PRECISION];
    // rotation (quaternion)
    float quat_x;
    float quat_y;
    float quat_z;
    float quat_w;
    char r_quat_x[NUMBER_PRECISION];
    char r_quat_y[NUMBER_PRECISION];
    char r_quat_z[NUMBER_PRECISION];
    char r_quat_w[NUMBER_PRECISION];
    //color
    char color[NUMBER_PRECISION];
    // transparency
    float transparency;
    char r_transparency[NUMBER_PRECISION];

    short modified_mask = 0; // Mask to track which parameters have been modified
    /**
     * @brief Génère le message formaté pour téléplot
     *
     * @return const char* contenant le message formaté
     */
    const char * generateTeleplotMessage(unsigned long temps);
};


#endif //TEAM2025_SPHERE_H
