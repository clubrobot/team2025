//
// Created by boris on 5/1/25.
//

#include "ihm.h"

#include <FreeRTOS.h>
#include <My_Clock.h>
#include <FreeRTOS/Source/include/task.h>

static LedButton bleu = LedButton(BLEU_BTN_PIN, INPUT_PULLUP, BLEU_LED_PIN);
static LedButton jaune = LedButton(JAUNE_BTN_PIN, INPUT_PULLUP, JAUNE_LED_PIN);
static LedButton vert = LedButton(VERT_BTN_PIN, INPUT_PULLUP, VERT_LED_PIN);
static LedButton rouge = LedButton(ROUGE_BTN_PIN, INPUT_PULLUP, ROUGE_LED_PIN);

static Buttons endstop_haut = Buttons(END_STOP_HAUT_PIN, INPUT_PULLDOWN);
static Buttons endstop_bas = Buttons(END_STOP_BAS_PIN, INPUT_PULLUP);
static Buttons tirette = Buttons(TIRETTE_PIN, INPUT_PULLUP);

namespace ihm {
    bool etat_bleu() { return !bleu.getState(); }

    bool etat_jaune() { return !jaune.getState(); }

    bool etat_vert() { return !vert.getState(); }

    bool etat_rouge() { return !rouge.getState(); }

    bool etat_lim_bas() { return !endstop_bas.getState(); }

    bool etat_lim_haut() { return endstop_haut.getState(); }

    bool etat_tirette() { return !tirette.getState(); }

    void led_bleu(bool etat) { bleu.setLedState(etat); }

    void led_jaune(bool etat) { jaune.setLedState(etat); }

    void led_vert(bool etat) { vert.setLedState(etat); }

    void led_rouge(bool etat) { rouge.setLedState(etat); }

    void led_bleu() { bleu.toggleLedState(); }

    void led_jaune() { jaune.toggleLedState(); }

    void led_vert() { vert.toggleLedState(); }

    void led_rouge() { rouge.toggleLedState(); }
}