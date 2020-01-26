#include <stdio.h>
#include <gtk/gtk.h>
#include <pthread.h>
#include <unistd.h>

#include "vcc3200.h"
#include "hw_types.h"
#include "hw_memmap.h"
#include "hw_gpio.h"
#include "pin.h"
#include "gpio.h"
#include "prcm.h"

#define SW2_PIN PIN_15
#define SW3_PIN PIN_04

static void sw2 (GtkWidget *widget, gpointer   data)
{
  setPin(SW2_PIN, 0xff);
  sleep (1);
  setPin(SW2_PIN, 0x00);
}

static void sw3 (GtkWidget *widget, gpointer   data)
{
  setPin(SW3_PIN, 0xff);
  sleep (1);
  setPin(SW3_PIN, 0x00);
}

// DO NOT KNOW HOW TO USE GTK 3 AS OF YET LOL
// Taken from the tutorial doc as a temporary solution for gui
static void activate (GtkApplication *app,
          gpointer        user_data)
{
  GtkWidget *window;
  GtkWidget *button;
  GtkWidget *button2;
  GtkWidget *button_box;

  window = gtk_application_window_new (app);
  gtk_window_set_title (GTK_WINDOW (window), "Window");
  gtk_window_set_default_size (GTK_WINDOW (window), 200, 200);

  button_box = gtk_button_box_new (GTK_ORIENTATION_HORIZONTAL);
  gtk_container_add (GTK_CONTAINER (window), button_box);

  button = gtk_button_new_with_label ("SW2");
  g_signal_connect (button, "clicked", G_CALLBACK (sw2), NULL);
  //g_signal_connect_swapped (button, "clicked", G_CALLBACK (gtk_widget_destroy), window);
  gtk_container_add (GTK_CONTAINER (button_box), button);

  button2 = gtk_button_new_with_label ("SW3");
  g_signal_connect (button2, "clicked", G_CALLBACK (sw3), NULL);
  //g_signal_connect_swapped (button, "clicked", G_CALLBACK (gtk_widget_destroy), window);
  gtk_container_add (GTK_CONTAINER (button_box), button2);

  gtk_widget_show_all (window);
}

static void *guiThread(void *var){
    GtkApplication *app;
    int status;

    app = gtk_application_new ("org.gtk.example", G_APPLICATION_FLAGS_NONE);
    g_signal_connect (app, "activate", G_CALLBACK (activate), NULL);
    status = g_application_run (G_APPLICATION (app), 0, 0);
    g_object_unref (app);

    return NULL;
}

void init(void) {
    printf("initialized virtual cpu\n");
    for (int i = PIN_01; i <= PIN_64; i++) {
        PinModeSet(i, PIN_MODE_0);
    }
    /* this variable is our reference to the second thread */
    pthread_t thread;

    pthread_create(&thread, NULL, guiThread, NULL); 
    //pthread_join(thread, NULL); 
}