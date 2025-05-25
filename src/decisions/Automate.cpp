//
// Created by bapti on 04/11/2024.
//

#include "Automate.h"
#include "My_Clock.h"
#include <chrono>
#include <FreeRTOS.h>
#include <FreeRTOSConfig_Default.h>
#include <Wheeledbase.h>
#include <FreeRTOS/Source/include/task.h>
#include <team2025/ListeActionneurs.h>

#include "Geogebra.h"
#include "variables_globales.h"
#include "sensors/SensorsThread.h"

#include "team2025/TacheEmpiler.h"
#include "team2025/TacheBanderole.h"


#include <Teleplot.h>

namespace Automate {
    Logger auto_logs = Logger("AUTOMATE");
    Tache* taches[100];//endroit de stockage des taches
    int color;//couleur de l'équipe
    int points;//points réalises pour l'instant
    int numberTaches;
}

void Automate::init(int team) {
    color=team;
    if(team==TEAM_JAUNE){
        auto_logs.log(WARNING_LEVEL, "Automate init avec Jaune\n");
        //import geogebra.h jaune

        positions_match=positions_jaune;
        Wheeledbase::SET_POSITION(&positions_match[start2]);

        numberTaches=2;
        taches[0] = new TacheBanderole();
        taches[1] = new TacheGoto((Position*)nullptr, &positions_match[garage4all], PurePursuit::FORWARD);
        //taches[0]=new TacheGoto(nullptr,&positions_match[garage81all], &positions_match[depot2]);

    }else{
        auto_logs.log(INFO_LEVEL, "Automate init avec Bleu\n");
        //import geogebra.h bleu
        positions_match=positions_bleu;

        Wheeledbase::SET_POSITION(&positions_match[start2]);

        numberTaches=1;
        taches[0] = new TacheBanderole();
        //taches[0]=new TacheGoto(nullptr,&positions_match[garage81all], &positions_match[depot2]);

    }
    //mettre les tâches a éxécuter ici.

}

void Automate::play_match(void *pvParameters){
    /*
    listeActionneur::asc_down();
    listeActionneur::pince_gauche.move(limite_pince_gauche.mi_non_deploye);
    poly_delay(100);
    listeActionneur::pince_droite.move(limite_pince_droite.mi_non_deploye);
    poly_delay(500);
    ServosPCA9685::Write(listeActionneur::servo_pince_droite, limite_servo_pince_droite.non_deploye);
    poly_delay(100);
    ServosPCA9685::Write(listeActionneur::servo_pince_gauche, limite_servo_pince_gauche.non_deploye);
    poly_delay(500);
    listeActionneur::pince_gauche.move(limite_pince_gauche.deploye);
    poly_delay(100);
    listeActionneur::pince_droite.move(limite_pince_droite.deploye);
    poly_delay(500);
    listeActionneur::set_pompe(HIGH);
    poly_delay(100);
    listeActionneur::asc_up();
    poly_delay(500);
    listeActionneur::pince_gauche.move(limite_pince_gauche.mi_non_deploye);
    poly_delay(100);
    listeActionneur::pince_droite.move(limite_pince_droite.mi_non_deploye);
    poly_delay(500);
    ServosPCA9685::Write(listeActionneur::servo_pince_droite, limite_servo_pince_droite.deploye);
    poly_delay(100);
    ServosPCA9685::Write(listeActionneur::servo_pince_gauche, limite_servo_pince_gauche.deploye);
    listeActionneur::set_pompe(LOW);
    poly_delay(100);
    listeActionneur::pince_gauche.move(limite_pince_gauche.deploye);
    poly_delay(100);
    listeActionneur::pince_droite.move(limite_pince_droite.deploye);
    delay(500);
    */

    auto *procedure_demarrage = (void (*)()) pvParameters;
    //cette fonction remplit le vecteur taches avec des tâches. Elles seront executée dans l'ordre ou elles ont été ajoutée.
    //Seulement la fonction execute  et get_necessary_time doivent être implémentée.
    //auto start_time = std::chrono::high_resolution_clock::now();
    procedure_demarrage();
    points=0;
    bool state=true;
    for (int tache_id = 0; tache_id < numberTaches; ++tache_id) {
        auto_logs.log(INFO_LEVEL, "Tache n.%d\n", tache_id);
        //execute les tâches dans l'ordre tant qu'on a assez de temps.
        //auto current = std::chrono::high_resolution_clock::now();
        //auto delta_t = std::chrono::duration<double, std::milli>(current-start_time).count();
        //auto_logs.log(INFO_LEVEL, "delta_t %f  et %f\n", delta_t, 100000-taches[tache_id]->get_necessary_time());
        //if(delta_t>100000-taches[tache_id]->get_necessary_time())break;//on est deja a la fin du match faut s'arrêter la
        //execute la tâche.
        state=taches[tache_id]->execute(state);
        //points+=taches[tache_id]->get_max_score();
    }

}