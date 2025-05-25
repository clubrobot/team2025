//
// Created by awing on 25/05/25.
//

#include "Teleplot_client.h"

#ifndef TEAM2025_CUBE_H
#define TEAM2025_CUBE_H


class Cube {
public:
    /**
     * @Brief Créé un cube pour le téléplot
     *
     * @param nom_forme
     * @param pos_x
     * @param pos_y
     * @param pos_z
     * @param width
     * @param height
     * @param depth
     * @param quat_x
     * @param quat_y
     * @param quat_z
     * @param quat_w
     * @param color
     * @param transparency
     */
    Cube(char* nom_forme, float pos_x, float pos_y, float pos_z,
         float width, float height, float depth,
         float quat_x, float quat_y, float quat_z, float quat_w,
         char* color, float transparency);

    /**
     * @brief modifie la position du cube
     *
     * @param pos_x
     * @param pos_y
     * @param pos_z
     */
    void setPosition(float pos_x, float pos_y, float pos_z);

    /**
     * @brief modifie la position x du cube
     *
     * @param pos_x
     */
    void setXPosition(float pos_x);

    /**
     * @brief modifie la position y du cube
     *
     * @param pos_y
     */
    void setYPosition(float pos_y);

    /**
     * @brief modifie la position z du cube
     *
     * @param pos_z
     */
    void setZPosition(float pos_z);

    /**
     * @brief modifie la taille du cube
     *
     * @param width
     * @param height
     * @param depth
     */
    void setSize(float width, float height, float depth);

    /**
     * @brief modifie la largeur du cube
     *
     * @param width
     */
    void setWidth(float width);

    /**
     * @brief modifie la hauteur du cube
     *
     * @param height
     */
    void setHeight(float height);

    /**
     * @brief modifie la profondeur du cube
     *
     * @param depth
     */
    void setDepth(float depth);

    /**
     * @brief modifie le quaternion de rotation du cube
     *
     * @param quat_x
     * @param quat_y
     * @param quat_z
     * @param quat_w
     */
    void setRotation(float quat_x, float quat_y, float quat_z, float quat_w);

    /**
     * @brief modifie le quaternion de rotation x du cube
     *
     * @param quat_x
     */
    void setQuatX(float quat_x);

    /**
     * @brief modifie le quaternion de rotation y du cube
     *
     * @param quat_y
     */
    void setQuatY(float quat_y);

    /**
     * @brief modifie le quaternion de rotation z du cube
     *
     * @param quat_z
     */
    void setQuatZ(float quat_z);

    /**
     * @brief modifie le quaternion de rotation w du cube
     *
     * @param quat_w
     */
    void setQuatW(float quat_w);

    /**
     * @brief modifie la couleur du cube
     *
     * @param color
     */
    void setColor(char* color);

    /**
     * @brief modifie la transparence du cube
     *
     * @param transparency
     */
    void setTransparency(float transparency);

    /**
     * @brief envoie l'état actuel du cube à téléplot
     *
     * This function will send the current state of the cube to Teleplot, including all modified parameters.
     * It will only send the parameters that have been modified since the last call to this function.
     *
     * @param teleplot The Teleplot instance to which the cube data will be sent.
     */
    void sendCubeToTeleplot(Teleplot &teleplot);

private:
    static constexpr size_t NAME_LENGTH = INTERNAL_BUFFER_SIZE;
    static const char NUMBER_PRECISION = 20;
    const short MASK_NAME =         0b0000000000000001;
    const short MASK_POS_X =        0b0000000000000010;
    const short MASK_POS_Y =        0b0000000000000100;
    const short MASK_POS_Z =        0b0000000000001000;
    const short MASK_WIDTH =        0b0000000000010000;
    const short MASK_HEIGHT =       0b0000000000100000;
    const short MASK_DEPTH =        0b0000000001000000;
    const short MASK_QUAT_X =       0b0000000010000000;
    const short MASK_QUAT_Y =       0b0000000100000000;
    const short MASK_QUAT_Z =       0b0000001000000000;
    const short MASK_QUAT_W =       0b0000010000000000;
    const short MASK_COLOR =        0b0000100000000000;
    const short MASK_TRANSPARENCY = 0b0001000000000000;

    char nom_forme[NAME_LENGTH];
    // position
    float pos_x;
    float pos_y;
    float pos_z;

    char r_pos_x[NUMBER_PRECISION];
    char r_pos_y[NUMBER_PRECISION];
    char r_pos_z[NUMBER_PRECISION];
    // taille
    float width;
    float height;
    float depth;
    char r_width[NUMBER_PRECISION];
    char r_height[NUMBER_PRECISION];
    char r_depth[NUMBER_PRECISION];
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
     * @brief genère le message formaté pour téléplot
     *
     * @return const char* contenant le message formaté
     */
    const char * generateTeleplotMessage(unsigned long temps);
};


#endif //TEAM2025_CUBE_H
