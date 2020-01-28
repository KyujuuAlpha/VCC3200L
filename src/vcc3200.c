#include <stdio.h>
#include <gtk/gtk.h>
#include <pthread.h>
#include <unistd.h>

#include "stdbool.h"
#include "vcc3200.h"
#include "hw_types.h"
#include "hw_memmap.h"
#include "hw_gpio.h"
#include "pin.h"
#include "spi.h"
#include "gpio.h"
#include "gpio_if.h"
#include "prcm.h"
#include "config.h"
#include "oled.h"

// SWITCH STUFF
static void switch2Activated(GtkWidget *widget, gpointer data) {
  setPin(SW2_PIN, 0xff);
  usleep(SWITCH_DELAY * 1000);
  setPin(SW2_PIN, 0x00);
}

static void switch3Activated(GtkWidget *widget, gpointer data) {
  setPin(SW3_PIN, 0xff);
  usleep(SWITCH_DELAY * 1000);
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

// LED STUFF
static unsigned char led1, led2, led3;
static bool ledFlag;
static GdkRGBA green = {0.0, 1.0, 0.0, 1.0};
static GdkRGBA orange = {1.0, 0.7, 0.1, 1.0};
static GdkRGBA red = {1.0, 0.0, 0.0, 1.0};
static GdkRGBA off = {0.0, 0.0, 0.0, 1.0};

static void closeLedWindow(void) {
  ledFlag = false;
}

static gboolean ledEventLoop(gpointer user_data) {
  if (!ledFlag) {
    return G_SOURCE_REMOVE;
  }
  if (led1 != GPIO_IF_LedStatus(MCU_GREEN_LED_GPIO)) {
    led1 = GPIO_IF_LedStatus(MCU_GREEN_LED_GPIO);
    gtk_widget_queue_draw(user_data);
  }
  if (led2 != GPIO_IF_LedStatus(MCU_ORANGE_LED_GPIO)) {
    led2 = GPIO_IF_LedStatus(MCU_ORANGE_LED_GPIO);
    gtk_widget_queue_draw(user_data);
  }
  if (led3 != GPIO_IF_LedStatus(MCU_RED_LED_GPIO)) {
    led3 = GPIO_IF_LedStatus(MCU_RED_LED_GPIO);
    gtk_widget_queue_draw(user_data);
  }
  return G_SOURCE_CONTINUE;
}

static gboolean ledDraw(GtkWidget *widget, cairo_t *cr, gpointer data) {
  guint width, height;
  GtkStyleContext *context;

  context = gtk_widget_get_style_context(widget);

  width = gtk_widget_get_allocated_width(widget);
  height = gtk_widget_get_allocated_height(widget);

  gtk_render_background(context, cr, 0, 0, width, height);

  cairo_rectangle(cr, 0, 0, width / 3, height);
  gdk_cairo_set_source_rgba(cr, led1 ? &green : &off);
  cairo_fill (cr);

  cairo_rectangle(cr, width / 3 * 1, 0, width / 3, height);
  gdk_cairo_set_source_rgba(cr, led2 ? &orange : &off);
  cairo_fill (cr);

  cairo_rectangle(cr, width / 3 * 2, 0, width / 3, height);
  gdk_cairo_set_source_rgba(cr, led3 ? &red : &off);
  cairo_fill (cr);

  return FALSE;
}

static GtkWidget *createLEDWindow(GtkApplication *app) {
  GtkWidget *ledWindow;
  GtkWidget *ledArea;

  led1 = GPIO_IF_LedStatus(MCU_GREEN_LED_GPIO);
  led2 = GPIO_IF_LedStatus(MCU_ORANGE_LED_GPIO);
  led3 = GPIO_IF_LedStatus(MCU_RED_LED_GPIO);

  ledWindow = gtk_application_window_new(app);
  gtk_window_set_title(GTK_WINDOW(ledWindow), "LEDs");
  gtk_window_set_default_size(GTK_WINDOW (ledWindow), 300, 100);

  ledArea = gtk_drawing_area_new();
  gtk_widget_set_size_request(ledArea, 100, 100);
  g_signal_connect(G_OBJECT(ledArea), "draw", G_CALLBACK(ledDraw), NULL);
  gtk_container_add(GTK_CONTAINER(ledWindow), ledArea);

  g_timeout_add(LED_TIMEOUT, ledEventLoop, ledWindow);
  
  g_signal_connect(ledWindow, "destroy", G_CALLBACK(closeLedWindow), NULL);
  ledFlag = true;

  return ledWindow;
}

// OLED STUFF
struct OLEDPixel {
  GdkRGBA color;
  bool change;
};
static bool oledFlag;
static int x1 = 0, x2 = 0, y1 = 0, y2 = 0, currX = 0, currY = 0;
static unsigned char cmd = 0, data = 0, color = 0;
static struct OLEDPixel display[OLED_HEIGHT][OLED_WIDTH];


static void closeOledWindow(void) {
  oledFlag = false;
}

static gboolean oledEventLoop(gpointer user_data) {
  if (!oledFlag) {
    return G_SOURCE_REMOVE;
  }
  for (int i = 0; i < OLED_WIDTH; i++) {
    for (int j = 0; j < OLED_HEIGHT; j++) {
      if(display[j][i].change) {
        gtk_widget_queue_draw(user_data);
      }
    }
  }
  return G_SOURCE_CONTINUE;
}

static void *oledProcessingThread(void *var) {
  unsigned long pin = 0;
  unsigned char val = 0, dcVal = 0, mosiVal = 0;
  bool dcFlag = false, pinFlag = false, processed = false;
  while (oledFlag) {
    while (processPinEntry(&pin, &val)) {
      processed = true;
      pinFlag = PIN_FLAG & pin;
      pin = ~PIN_FLAG & pin;
      if (pin == OLED_DC && pinFlag) {
        dcFlag = true;
        dcVal = val;
      }
      if (!dcFlag && pin == OLED_MOSI && !pinFlag) {
        mosiVal = val;
      } else if (dcFlag && pin == OLED_MOSI && !pinFlag && cmd) {
        if (cmd == CMD_SETCOLUMN) {
          if (data == 0) {
            x1 = val > OLED_WIDTH ? OLED_WIDTH - 1 : val < 0 ? 0 : val;
            currX = x1;
            data = 1;
          } else if (data == 1) {
            x2 = val > OLED_WIDTH ? OLED_WIDTH - 1 : val < 0 ? 0 : val;
          }
        } else if (cmd == CMD_SETROW) {
          if (data == 0) {
            y1 = val > OLED_HEIGHT ? OLED_HEIGHT - 1 : val < 0 ? 0 : val;
            currY = y1;
            data = 1;
          } else if (data == 1) {
            y2 = val > OLED_HEIGHT ? OLED_HEIGHT - 1 : val < 0 ? 0 : val;
          }
        } else if (cmd == CMD_WRITERAM) {
          if (data == 0) {
            data = 1;
          } else if (data == 1) {
            color = val;
            data = 0;
            display[currY][currX].change = true;
            display[currY][currX].color.red = ((color & 0xF800) >> 11) / 31;
            display[currY][currX].color.green = ((color & 0x07E0) >> 5) / 63;
            display[currY][currX].color.blue = (color & 0x001F) / 31;
            currX++;
            if (currX > x2) {
              currX = x1;
              currY++;
              if (currY > y2) {
                currY=y1;
              }
            }
          }
        }
      }
    }
    if (!dcFlag && dcVal == 0 && processed) {
      cmd = mosiVal;
      data = 0;
    }
    pin = 0;
    val = 0;
    dcVal = 0;
    mosiVal = 0;
    dcFlag = false;
    pinFlag = false;
    processed = false;
  }
  return NULL;
}

static gboolean oledDraw(GtkWidget *widget, cairo_t *cr, gpointer data) {
  guint width, height;
  GtkStyleContext *context;

  context = gtk_widget_get_style_context(widget);

  width = gtk_widget_get_allocated_width(widget);
  height = gtk_widget_get_allocated_height(widget);

  gtk_render_background(context, cr, 0, 0, width, height);

  //draw 
  for (int i = 0; i < OLED_WIDTH; i++) {
    for (int j = 0; j < OLED_HEIGHT; j++) {
      cairo_rectangle(cr, i * (width / OLED_WIDTH), j * (height / OLED_HEIGHT), (width / OLED_WIDTH), (height / OLED_HEIGHT));
      gdk_cairo_set_source_rgba(cr, &display[j][i].color);
      cairo_fill (cr);
      display[j][i].change = false;
    }
  } 

  return FALSE;
}

static GtkWidget *createOLEDWindow(GtkApplication *app) {
  GtkWidget *oledWindow;
  GtkWidget *oledArea;

  oledWindow = gtk_application_window_new(app);
  gtk_window_set_title(GTK_WINDOW(oledWindow), "OLED");
  gtk_window_set_default_size(GTK_WINDOW (oledWindow), OLED_WIDTH * 2, OLED_HEIGHT * 2);

  oledArea = gtk_drawing_area_new();
  gtk_widget_set_size_request(oledArea, OLED_WIDTH * 2, OLED_HEIGHT * 2);
  g_signal_connect(G_OBJECT(oledArea), "draw", G_CALLBACK(oledDraw), NULL);
  gtk_container_add(GTK_CONTAINER(oledWindow), oledArea);

  g_timeout_add(OLED_TIMEOUT, oledEventLoop, oledWindow);
  
  g_signal_connect(oledWindow, "destroy", G_CALLBACK(closeOledWindow), NULL);
  oledFlag = true;

  for (int i = 0; i < OLED_WIDTH; i++) {
    for (int j = 0; j < OLED_HEIGHT; j++) {
      display[j][i] = (struct OLEDPixel) { (GdkRGBA) { 0.0, 0.0, 0.0, 1.0 }, true };
    }
  }

  pthread_t thread;
  pthread_create(&thread, NULL, oledProcessingThread, NULL); 

  return oledWindow;
  }

// MAIN GUI STUFF
static void gtkInit(GtkApplication *app, gpointer user_data) {
  if (ENABLE_SWITCHES) {
    GtkWidget *switchWindow = createSwitchWindow(app);
    gtk_widget_show_all(switchWindow);
  }
  if (ENABLE_LEDS) {
    GtkWidget *ledWindow = createLEDWindow(app);
    gtk_widget_show_all(ledWindow);
  }
  if (ENABLE_OLED) {
    GtkWidget *oledWindow = createOLEDWindow(app);
    gtk_widget_show_all(oledWindow);
  }
}

static void *guiThread(void *var) {
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