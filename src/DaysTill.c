/*
"Classic" Digital Watch Pebble App
*/
// Standard includes
#include "pebble.h"
#include <PDUtils.h>

// App-specific data
Window *window; // All apps must have at least one window
TextLayer *time_layer; // The clock
TextLayer *some_text;
TextLayer *days_text;


char countText[8];


// Called once per second
static void handle_second_tick(struct tm* tick_time, TimeUnits units_changed) {

  static char time_text[] = "000000"; // Needs to be static because it's used by the system later.
  static int EVENT_MONTH = 4;
  static int EVENT_DAY = 18;
  static int EVENT_YEAR = 14;
  static int EVENT_HOUR = 12;
  static int EVENT_MINUTE = 0;
  //static char vt_hacks[] = "00:00:00";
  time_t rawtime = time(NULL);
  struct tm* test_time = localtime(&rawtime);


  time_t eventNow = p_mktime(test_time);

  test_time->tm_mon = EVENT_MONTH - 1;
  test_time->tm_mday = EVENT_DAY;
  test_time->tm_year = EVENT_YEAR + 100;
  test_time->tm_hour = EVENT_HOUR;
  test_time->tm_min = EVENT_MINUTE;
  test_time->tm_sec = 0; 


  time_t eventFuture = p_mktime(test_time);
  

  //int hoursTill = (((eventFuture - eventNow) / 60 ) / 60)/24; 
  int diff = (eventFuture - eventNow);
  int secondsTill = diff % 60;
  diff = diff/60;
  int minutesTill = diff % 60;
  diff = diff/60;
  int hoursTill = diff % 24; 
  diff = diff/24;

  static char countText[] = "";

  snprintf(countText, 10000, "%d", diff);
  //strftime(time_text, sizeof(time_text), "%T", test_time);
  text_layer_set_text(time_layer, countText);

  //strftime(vt_hacks, sizeof(vt_hacks), "%T", tick_time);
  text_layer_set_text(some_text, "Time till VTHacks");
  text_layer_set_text(days_text, "Days");
}


// Handle the start-up of the app
static void do_init(void) {

  // Create our app's base window
  window = window_create();
  window_stack_push(window, true);
  window_set_background_color(window, GColorBlack);

  // Init the text layer used to show the time
  time_layer = text_layer_create(GRect(30, 75, 144-40 /* width */, 168-54 /* height */));
  text_layer_set_text_color(time_layer, GColorWhite);
  text_layer_set_background_color(time_layer, GColorClear);
  text_layer_set_font(time_layer, fonts_get_system_font(FONT_KEY_GOTHIC_28_BOLD));

  some_text = text_layer_create(GRect(30, 10, 144-40 /* width */, 168-54 /* height */));
  text_layer_set_text_color(some_text, GColorWhite);
  text_layer_set_background_color(some_text, GColorClear);
  text_layer_set_font(some_text, fonts_get_system_font(FONT_KEY_GOTHIC_28_BOLD));

  days_text = text_layer_create(GRect(60, 75, 144-40 /* width */, 168-54 /* height */));
  text_layer_set_text_color(days_text, GColorWhite);
  text_layer_set_background_color(days_text, GColorClear);
  text_layer_set_font(days_text, fonts_get_system_font(FONT_KEY_GOTHIC_28_BOLD));

  // Ensures time is displayed immediately (will break if NULL tick event accessed).
  // (This is why it's a good idea to have a separate routine to do the update itself.)
  time_t now = time(NULL);
  struct tm *current_time = localtime(&now);


  handle_second_tick(current_time, SECOND_UNIT);
  tick_timer_service_subscribe(SECOND_UNIT, &handle_second_tick);


  layer_add_child(window_get_root_layer(window), text_layer_get_layer(time_layer));

  layer_add_child(window_get_root_layer(window), text_layer_get_layer(some_text));
  layer_add_child(window_get_root_layer(window), text_layer_get_layer(days_text));

}

static void do_deinit(void) {
  text_layer_destroy(time_layer);
  text_layer_destroy(some_text);
  window_destroy(window);
}

// The main event/run loop for our app
int main(void) {
  do_init();
  app_event_loop();
  do_deinit();
}
