//
// Created by awing on 25/05/25.
//

#include "Cube.h"

Cube::Cube(char* nom_forme, float pos_x, float pos_y, float pos_z,
           float width, float height, float depth,
           float quat_x, float quat_y, float quat_z, float quat_w,
           char* color, float transparency) {
    strcpy(this->nom_forme, nom_forme);
    this->pos_x = pos_x;
    this->pos_y = pos_y;
    this->pos_z = pos_z;
    this->width = width;
    this->height = height;
    this->depth = depth;
    this->quat_x = quat_x;
    this->quat_y = quat_y;
    this->quat_z = quat_z;
    this->quat_w = quat_w;
    strcpy(this->color, color);
    this->transparency = transparency;
    char voidBuf[INTERNAL_BUFFER_SIZE] = "";
    char buf[INTERNAL_BUFFER_SIZE] = "";
    snprintf(buf, INTERNAL_BUFFER_SIZE, "%f", pos_x);
    strcpy(this->r_pos_x, buf);
    strcpy(buf, voidBuf);
    snprintf(buf, INTERNAL_BUFFER_SIZE, "%f", pos_y);
    strcpy(this->r_pos_y, buf);
    strcpy(buf, voidBuf);
    snprintf(buf, INTERNAL_BUFFER_SIZE, "%f", pos_z);
    strcpy(this->r_pos_z, buf);
    strcpy(buf, voidBuf);
    snprintf(buf, INTERNAL_BUFFER_SIZE, "%f", width);
    strcpy(this->r_width, buf);
    strcpy(buf, voidBuf);
    snprintf(buf, INTERNAL_BUFFER_SIZE, "%f", height);
    strcpy(this->r_height, buf);
    strcpy(buf, voidBuf);
    snprintf(buf, INTERNAL_BUFFER_SIZE, "%f", depth);
    strcpy(this->r_depth, buf);
    strcpy(buf, voidBuf);
    snprintf(buf, INTERNAL_BUFFER_SIZE, "%f", quat_x);
    strcpy(this->r_quat_x, buf);
    strcpy(buf, voidBuf);
    snprintf(buf, INTERNAL_BUFFER_SIZE, "%f", quat_y);
    strcpy(this->r_quat_y, buf);
    strcpy(buf, voidBuf);
    snprintf(buf, INTERNAL_BUFFER_SIZE, "%f", quat_z);
    strcpy(this->r_quat_z, buf);
    strcpy(buf, voidBuf);
    snprintf(buf, INTERNAL_BUFFER_SIZE, "%f", quat_w);
    strcpy(this->r_quat_w, buf);
    strcpy(buf, voidBuf);
    snprintf(buf, INTERNAL_BUFFER_SIZE, "%f", transparency);
    strcpy(this->r_transparency, buf);
    this->modified_mask |= (MASK_NAME | MASK_POS_X | MASK_POS_Y | MASK_POS_Z |
                            MASK_WIDTH | MASK_HEIGHT | MASK_DEPTH |
                            MASK_QUAT_X | MASK_QUAT_Y | MASK_QUAT_Z | MASK_QUAT_W |
                            MASK_COLOR | MASK_TRANSPARENCY);
}

void Cube::setPosition(float x, float y, float z) {
    char buf[INTERNAL_BUFFER_SIZE] = "";
    char voidBuf[INTERNAL_BUFFER_SIZE] = "";
    this->pos_x = x;
    snprintf(buf, INTERNAL_BUFFER_SIZE, "%f", x);
    strcpy(this->r_pos_x, buf);
    strcpy(buf, voidBuf);
    this->pos_y = y;
    snprintf(buf, INTERNAL_BUFFER_SIZE, "%f", y);
    strcpy(this->r_pos_y, buf);
    strcpy(buf, voidBuf);
    this->pos_z = z;
    snprintf(buf, INTERNAL_BUFFER_SIZE, "%f", z);
    strcpy(this->r_pos_z, buf);
    this->modified_mask |= (MASK_POS_X | MASK_POS_Y | MASK_POS_Z);
}
void Cube::setXPosition(float x) {
    char buf[INTERNAL_BUFFER_SIZE] = "";
    this->pos_x = x;
    snprintf(buf, INTERNAL_BUFFER_SIZE, "%f", x);
    strcpy(this->r_pos_x, buf);
    this->modified_mask |= MASK_POS_X;
}
void Cube::setYPosition(float y) {
    char buf[INTERNAL_BUFFER_SIZE] = "";
    this->pos_y = y;
    snprintf(buf, INTERNAL_BUFFER_SIZE, "%f", y);
    strcpy(this->r_pos_y, buf);
    this->modified_mask |= MASK_POS_Y;
}
void Cube::setZPosition(float z) {
    char buf[INTERNAL_BUFFER_SIZE] = "";
    this->pos_z = z;
    snprintf(buf, INTERNAL_BUFFER_SIZE, "%f", z);
    strcpy(this->r_pos_z, buf);
    this->modified_mask |= MASK_POS_Z;
}
void Cube::setSize(float w, float h, float d) {
    char buf[INTERNAL_BUFFER_SIZE] = "";
    char voidBuf[INTERNAL_BUFFER_SIZE] = "";
    this->width = w;
    snprintf(buf, INTERNAL_BUFFER_SIZE, "%f", w);
    strcpy(this->r_width, buf);
    strcpy(buf, voidBuf);
    this->height = h;
    snprintf(buf, INTERNAL_BUFFER_SIZE, "%f", h);
    strcpy(this->r_height, buf);
    strcpy(buf, voidBuf);
    this->depth = d;
    snprintf(buf, INTERNAL_BUFFER_SIZE, "%f", d);
    strcpy(this->r_depth, buf);
    this->modified_mask |= (MASK_WIDTH | MASK_HEIGHT | MASK_DEPTH);
}
void Cube::setWidth(float w) {
    char buf[INTERNAL_BUFFER_SIZE] = "";
    this->width = w;
    snprintf(buf, INTERNAL_BUFFER_SIZE, "%f", w);
    strcpy(this->r_width, buf);
    this->modified_mask |= MASK_WIDTH;
}
void Cube::setHeight(float h) {
    char buf[INTERNAL_BUFFER_SIZE] = "";
    this->height = h;
    snprintf(buf, INTERNAL_BUFFER_SIZE, "%f", h);
    strcpy(this->r_height, buf);
    this->modified_mask |= MASK_HEIGHT;
}
void Cube::setDepth(float d) {
    char buf[INTERNAL_BUFFER_SIZE] = "";
    this->depth = d;
    snprintf(buf, INTERNAL_BUFFER_SIZE, "%f", d);
    strcpy(this->r_depth, buf);
    this->modified_mask |= MASK_DEPTH;
}
void Cube::setRotation(float x, float y, float z, float w) {
    char buf[INTERNAL_BUFFER_SIZE] = "";
    char voidBuf[INTERNAL_BUFFER_SIZE] = "";
    this->quat_x = x;
    snprintf(buf, INTERNAL_BUFFER_SIZE, "%f", x);
    strcpy(this->r_quat_x, buf);
    strcpy(buf, voidBuf);
    this->quat_y = y;
    snprintf(buf, INTERNAL_BUFFER_SIZE, "%f", y);
    strcpy(this->r_quat_y, buf);
    strcpy(buf, voidBuf);
    this->quat_z = z;
    snprintf(buf, INTERNAL_BUFFER_SIZE, "%f", z);
    strcpy(this->r_quat_z, buf);
    strcpy(buf, voidBuf);
    this->quat_w = w;
    snprintf(buf, INTERNAL_BUFFER_SIZE, "%f", w);
    strcpy(this->r_quat_w, buf);
    this->modified_mask |= (MASK_QUAT_X | MASK_QUAT_Y | MASK_QUAT_Z | MASK_QUAT_W);
}
void Cube::setQuatX(float x) {
    char buf[INTERNAL_BUFFER_SIZE] = "";
    this->quat_x = x;
    snprintf(buf, INTERNAL_BUFFER_SIZE, "%f", x);
    strcpy(this->r_quat_x, buf);
    this->modified_mask |= MASK_QUAT_X;
}
void Cube::setQuatY(float y) {
    char buf[INTERNAL_BUFFER_SIZE] = "";
    this->quat_y = y;
    snprintf(buf, INTERNAL_BUFFER_SIZE, "%f", y);
    strcpy(this->r_quat_y, buf);
    this->modified_mask |= MASK_QUAT_Y;
}
void Cube::setQuatZ(float z) {
    char buf[INTERNAL_BUFFER_SIZE] = "";
    this->quat_z = z;
    snprintf(buf, INTERNAL_BUFFER_SIZE, "%f", z);
    strcpy(this->r_quat_z, buf);
    this->modified_mask |= MASK_QUAT_Z;
}
void Cube::setQuatW(float w) {
    char buf[INTERNAL_BUFFER_SIZE] = "";
    this->quat_w = w;
    snprintf(buf, INTERNAL_BUFFER_SIZE, "%f", w);
    strcpy(this->r_quat_w, buf);
    this->modified_mask |= MASK_QUAT_W;
}
void Cube::setColor(char* c) {
    strcpy(this->color, c);
    this->modified_mask |= MASK_COLOR;
}
void Cube::setTransparency(float t) {
    char buf[INTERNAL_BUFFER_SIZE] = "";
    this->transparency = t;
    snprintf(buf, INTERNAL_BUFFER_SIZE, "%f", t);
    strcpy(this->r_transparency, buf);
    this->modified_mask |= MASK_TRANSPARENCY;
}

void Cube::sendCubeToTeleplot(Teleplot &teleplot) {
    teleplot.ajout_ou_envoie_tampon(this->generateTeleplotMessage(teleplot.get_temps()));
}

const char* Cube::generateTeleplotMessage(unsigned long temps) {
    static char buf[INTERNAL_BUFFER_SIZE];
    snprintf(buf, INTERNAL_BUFFER_SIZE, ">3d|%s:%lu:S:cube:P:%s:%s:%s:Q:%s:%s:%s:%s:W:%s:H:%s:D:%s:C:%s:O:%s\n",
             this->nom_forme,
             temps,
             (this->modified_mask & MASK_POS_X) ? this->r_pos_x : "",
             (this->modified_mask & MASK_POS_Y) ? this->r_pos_y : "",
             (this->modified_mask & MASK_POS_Z) ? this->r_pos_z : "",
             (this->modified_mask & MASK_QUAT_X) ? this->r_quat_x : "",
             (this->modified_mask & MASK_QUAT_Y) ? this->r_quat_y : "",
             (this->modified_mask & MASK_QUAT_Z) ? this->r_quat_z : "",
             (this->modified_mask & MASK_QUAT_W) ? this->r_quat_w : "",
             (this->modified_mask & MASK_WIDTH) ? this->r_width : "",
             (this->modified_mask & MASK_HEIGHT) ? this->r_height : "",
             (this->modified_mask & MASK_DEPTH) ? this->r_depth : "",
             (this->modified_mask & MASK_COLOR) ? this->color : "",
             (this->modified_mask & MASK_TRANSPARENCY) ? this->r_transparency : "");
    return buf;
}