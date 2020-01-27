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
#include "config.h"

static void switch2Activated(GtkWidget *widget, gpointer data) {
  setPin(SW2_PIN, 0xff);
  usleep(SWITCH_DELAY);
  setPin(SW2_PIN, 0x00);
}

static void switch3Activated(GtkWidget *widget, gpointer data) {
  setPin(SW3_PIN, 0xff);
  usleep(SWITCH_DELAY);
  setPin(SW3_PIN, 0x00);
}

static GtkWidget *createSwitchWindow(GtkApplication *app) {
  GtkWidget *switchWindow;
  GtkWidget *switchBox;
  GtkWidget *sw2Button, *sw3Button;

  switchWindow = gtk_application_window_new(app);
  gtk_window_set_title(GTK_WINDOW(switchWindow), "Switches");
  gtk_window_set_default_size(GTK_WINDOW (switchWindow), 200, 100);

  switchBox = gtk_button_box_new(GTK_ORIENTATION_HORIZONTAL);
  gtk_container_add(GTK_CONTAINER(switchWindow), switchBox);

  sw2Button = gtk_button_new_with_label("SW2");
  g_signal_connect(sw2Button, "clicked", G_CALLBACK(switch2Activated), NULL);

  sw3Button = gtk_button_new_with_label("SW3");
  g_signal_connect(sw3Button, "clicked", G_CALLBACK(switch3Activated), NULL);

  gtk_container_add(GTK_CONTAINER(switchBox), sw2Button);
  gtk_container_add(GTK_CONTAINER(switchBox), sw3Button);

  return switchWindow;
}

static void gtkInit(GtkApplication *app, gpointer user_data) {
  if (ENABLE_SWITCHES) {
    GtkWidget *switchWindow = createSwitchWindow(app);
    gtk_widget_show_all(switchWindow);
  }
}

static void *guiThread(void *var){
  GtkApplication *app = gtk_application_new("com.troi.vcc3200", G_APPLICATION_FLAGS_NONE);
  g_signal_connect(app, "activate", G_CALLBACK(gtkInit), NULL);
  g_application_run(G_APPLICATION(app), 0, 0);
  g_object_unref(app);
  return NULL;
}

void init(void) {
  printf("initialized virtual cpu\n");
  for (int i = PIN_01; i <= PIN_64; i++) {
    PinModeSet(i, PIN_MODE_0);
  }

  pthread_t thread;
  if (ENABLE_GUI) {
    pthread_create(&thread, NULL, guiThread, NULL); 
  }
}