//
// Created by awing on 25/05/25.
//

#include "Sphere.h"

Sphere::Sphere(char* nom_forme, float pos_x, float pos_y, float pos_z,
               float radius, float quat_x, float quat_y, float quat_z, float quat_w,
               int precision, char* color, float transparency) {
    strcpy(this->nom_forme, nom_forme);
    this->pos_x = pos_x;
    this->pos_y = pos_y;
    this->pos_z = pos_z;
    this->radius = radius;
    this->quat_x = quat_x;
    this->quat_y = quat_y;
    this->quat_z = quat_z;
    this->quat_w = quat_w;
    this->precision = precision;
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

    snprintf(buf, INTERNAL_BUFFER_SIZE, "%f", radius);
    strcpy(this->r_radius, buf);
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

    snprintf(buf, INTERNAL_BUFFER_SIZE, "%d", precision);
    strcpy(this->r_precision, buf);
    strcpy(buf, voidBuf);

    snprintf(buf, INTERNAL_BUFFER_SIZE, "%f", transparency);
    strcpy(this->r_transparency, buf);
    this->modified_mask |= MASK_NAME | MASK_POS_X | MASK_POS_Y | MASK_POS_Z |
                          MASK_RADIUS | MASK_PRECISION | MASK_QUAT_X | MASK_QUAT_Y |
                          MASK_QUAT_Z | MASK_QUAT_W | MASK_COLOR | MASK_TRANSPARENCY;
}

void Sphere::setPosition(float x, float y, float z) {
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

void Sphere::setXPosition(float x) {
    char buf[INTERNAL_BUFFER_SIZE] = "";
    this->pos_x = x;
    snprintf(buf, INTERNAL_BUFFER_SIZE, "%f", x);
    strcpy(this->r_pos_x, buf);
    this->modified_mask |= MASK_POS_X;
}

void Sphere::setYPosition(float y) {
    char buf[INTERNAL_BUFFER_SIZE] = "";
    this->pos_y = y;
    snprintf(buf, INTERNAL_BUFFER_SIZE, "%f", y);
    strcpy(this->r_pos_y, buf);
    this->modified_mask |= MASK_POS_Y;
}

void Sphere::setZPosition(float z) {
    char buf[INTERNAL_BUFFER_SIZE] = "";
    this->pos_z = z;
    snprintf(buf, INTERNAL_BUFFER_SIZE, "%f", z);
    strcpy(this->r_pos_z, buf);
    this->modified_mask |= MASK_POS_Z;
}

void Sphere::setRadius(float r) {
    char buf[INTERNAL_BUFFER_SIZE] = "";
    this->radius = r;
    snprintf(buf, INTERNAL_BUFFER_SIZE, "%f", r);
    strcpy(this->r_radius, buf);
    this->modified_mask |= MASK_RADIUS;
}

void Sphere::setPrecision(int p){
    char buf[INTERNAL_BUFFER_SIZE] = "";
    this->precision = p;
    snprintf(buf, INTERNAL_BUFFER_SIZE, "%d", p);
    strcpy(this->r_precision, buf);
    this->modified_mask |= MASK_PRECISION;
}

void Sphere::setRotation(float x, float y, float z, float w) {
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

void Sphere::setQuatX(float x) {
    char buf[INTERNAL_BUFFER_SIZE] = "";
    this->quat_x = x;
    snprintf(buf, INTERNAL_BUFFER_SIZE, "%f", x);
    strcpy(this->r_quat_x, buf);
    this->modified_mask |= MASK_QUAT_X;
}

void Sphere::setQuatY(float y) {
    char buf[INTERNAL_BUFFER_SIZE] = "";
    this->quat_y = y;
    snprintf(buf, INTERNAL_BUFFER_SIZE, "%f", y);
    strcpy(this->r_quat_y, buf);
    this->modified_mask |= MASK_QUAT_Y;
}

void Sphere::setQuatZ(float z) {
    char buf[INTERNAL_BUFFER_SIZE] = "";
    this->quat_z = z;
    snprintf(buf, INTERNAL_BUFFER_SIZE, "%f", z);
    strcpy(this->r_quat_z, buf);
    this->modified_mask |= MASK_QUAT_Z;
}

void Sphere::setQuatW(float w) {
    char buf[INTERNAL_BUFFER_SIZE] = "";
    this->quat_w = w;
    snprintf(buf, INTERNAL_BUFFER_SIZE, "%f", w);
    strcpy(this->r_quat_w, buf);
    this->modified_mask |= MASK_QUAT_W;
}

void Sphere::setColor(const char* c) {
    strcpy(this->color, c);
    this->modified_mask |= MASK_COLOR;
}

void Sphere::setTransparency(float t){
    char buf[INTERNAL_BUFFER_SIZE] = "";
    this->transparency = t;
    snprintf(buf, INTERNAL_BUFFER_SIZE, "%f", t);
    strcpy(this->r_transparency, buf);
    this->modified_mask |= MASK_TRANSPARENCY;
}

const char* Sphere::generateTeleplotMessage(unsigned long temps) {
    static char message[INTERNAL_BUFFER_SIZE];
    snprintf(message, INTERNAL_BUFFER_SIZE,
             ">3d|%s:%lu:S:sphere:P:%s:%s:%s:RA:%s:PR:%s:Q:%s:%s:%s:%s:C:%s:O:%s\n",
             this->nom_forme,
             temps,
             (this->modified_mask & MASK_POS_X) ? this->r_pos_x : "",
             (this->modified_mask & MASK_POS_Y) ? this->r_pos_y : "",
             (this->modified_mask & MASK_POS_Z) ? this->r_pos_z : "",
             (this->modified_mask & MASK_RADIUS) ? this->r_radius : "",
             (this->modified_mask & MASK_PRECISION) ? this->r_precision : "",
             (this->modified_mask & MASK_QUAT_X) ? this->r_quat_x : "",
             (this->modified_mask & MASK_QUAT_Y) ? this->r_quat_y : "",
             (this->modified_mask & MASK_QUAT_Z) ? this->r_quat_z : "",
             (this->modified_mask & MASK_QUAT_W) ? this->r_quat_w : "",
             (this->modified_mask & MASK_COLOR) ? this->color : "",
             (this->modified_mask & MASK_TRANSPARENCY) ? this->r_transparency : ""
             );
    return message;
}

void Sphere::sendSphereToTeleplot(Teleplot &teleplot) {
    teleplot.ajout_ou_envoie_tampon(this->generateTeleplotMessage(teleplot.get_temps()));
}