#include <Adafruit_NeoPixel.h>
#include <BetterJoystick.h>
#include <LiquidCrystal.h>
#include <Wire.h>
#include <stdint.h>
#include "Tlc5940.h"
#include "SevSeg.h"
#include "special_char.h"


#define DEBUG 1
#define BAUD_RATE 9600
#define RAND_PIN A15

// -- LCD ------------------------------------------------------------------
#define ADD 1
#define SUBSTRACT 0

// BTN LCD CONTROL PIN
#define LEFT_BTN 49
#define RIGHT_BTN 46
#define UP_BTN 47
#define DOWN_BTN 48

// LCD PIN
#define LCD_HEIGHT 2
#define LCD_WIDTH 16
#define RS 7
#define EN 6
#define D4 5
#define D5 4
#define D6 3
#define D7 2

// LCD VARIABLES :
const String words[10] = {
  "DALEKS",
  "CYBERMEN",
  "TIMELORD",
  "TOYMAKER",
  "TARDIS",
  "TIMEANDSPACE",
  "BINARY",
  "LONDON",
  "REGENERATION",
  "SONTAR",
};

int8_t user_cursor = 0;
String selected_word;
String encrypted_word;
uint8_t word_length;
uint8_t center_offset;
uint8_t caesar_offset[2];
char input_word[16] = { '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0' };


// LCD FUNCTIONS :
LiquidCrystal lcd(RS, EN, D4, D5, D6, D7);
void detect_button_press();
void wait_btn_release(uint8_t button);
void print_input_word(void);
void update_char_value(uint8_t action);
void check_encryption(void);
String encryption(String word_to_encrypt, uint8_t gap);


// -- TIMER --------------------------------------------------------------
#define DIGIT_NUMBER 4
#define START_TIME 45
#define DISPLAY_BRIGHTNESS 100

#define SEG_D1 33
#define SEG_D2 32
#define SEG_D3 31
#define SEG_D4 30

#define SEG_A 23
#define SEG_B 24
#define SEG_C 25
#define SEG_D 26
#define SEG_E 27
#define SEG_F 28
#define SEG_G 29
#define SEG_DP 22


// TIMER VARIABLES :
uint32_t timer_start = millis();
char formatted_time[5];
const uint8_t DIGIT_PINS[] = { SEG_D1, SEG_D2, SEG_D3, SEG_D4 };
const uint8_t SEGMENT_PINS[] = {
  SEG_A,
  SEG_B,
  SEG_C,
  SEG_D,
  SEG_E,
  SEG_F,
  SEG_G,
  SEG_DP
};

SevSeg display;

// TIMER FUNCTIONS :
void decrement_time(void);
void time_blink(void);


// -- JOYSTICK MODULE ------------------------------------------------------
#define LED_COUNT 16
#define LED_PIN 44
#define MAX_BRIGHTNESS 75
#define RAND_PIN A15

#define J_X A1
#define J_Y A0

#define J_UP 1
#define J_DOWN 2
#define J_LEFT 3
#define J_RIGHT 4

#define ACTION -1
#define ERROR 1
#define SUCCESS 0
#define ACTION_ANIM_DELAY 50

#define DELAYED_DISPLAY 200
#define INSTANT_DISPLAY 0


// JOYSTICK VARIABLE :
int decimal_numbers[] = { 25772, 4757, 29294, 19348, 31011, 32063, 17884, 35933, 727, 29973 };
uint8_t BUILTIN_JOYSTICK_SEQ[10][4] = {
  { J_LEFT, J_UP, J_RIGHT, J_UP },
  { J_UP, J_RIGHT, J_LEFT, J_DOWN },
  { J_DOWN, J_RIGHT, J_UP, J_RIGHT },
  { J_LEFT, J_DOWN, J_UP, J_RIGHT },
  { J_RIGHT, J_LEFT, J_UP, J_DOWN },
  { J_DOWN, J_LEFT, J_RIGHT, J_UP },
  { J_RIGHT, J_DOWN, J_UP, J_LEFT },
  { J_UP, J_LEFT, J_UP, J_RIGHT },
  { J_LEFT, J_DOWN, J_RIGHT, J_UP },
  { J_DOWN, J_RIGHT, J_LEFT, J_DOWN },
};


int selected_joystick_number;
int8_t trig_rotation;
uint8_t selected_joystick_seq;
uint8_t joystick_input_seq[4];
uint8_t joystick_seq_index = 0;

// JOYSTICK FUNCTIONS :

Adafruit_NeoPixel strip(LED_COUNT, LED_PIN, NEO_GRB + NEO_KHZ800);
Joystick joystick(J_X, J_Y, -1);
void blink_animation(uint8_t blink_count, uint16_t duration, int8_t effect_id);
void show_binary_on_screen(int binary, int8_t trig_rotation, uint16_t effect_delay);
void add_direction_to_seq(uint8_t direction);
bool check_seq(uint8_t seq_id);
void event_joystick_listener(void);
void wait_joystick_release(void);
void idle_animation(void);
void check_win(void);


// -- WIRE MODULE ----------------------------------------------------------
#define TERM_COUNT 8
#define A_PIN 42
#define B_PIN 43
#define C_PIN 40
#define D_PIN 41
#define E_PIN 38
#define F_PIN 36
#define G_PIN 39
#define H_PIN 37

#define RED_LED_PIN 34
#define GREEN_LED_PIN 35

#define CUTTED 1
#define CONNECTED 0

// WIRE VARIABLE :
const uint8_t TERMINAL_PINS[8] = { A_PIN, B_PIN, C_PIN, D_PIN, E_PIN, F_PIN, G_PIN, H_PIN };
const uint8_t BUILTIN_WIRE_STATES[8] = { CUTTED, CUTTED, CONNECTED, CONNECTED, CUTTED, CONNECTED, CONNECTED, CUTTED };
uint8_t current_wire_state[8];

// WIRE FUNCTIONS :
void wire_disconnect_handler(void);
void check_wire_connection(void);


// -- BUZZER MODULE ------------------------------------------------------------
#define BUZZER_PIN 12

#define SOUND_ON_TIMER_TICK 1
#define TICK_FREQ 1000

#define SOUND_ON_ACTION 1
#define ACTION_FREQ 300

#define SOUND_ON_EVENT 1
#define SUCCESS_FREQ 783
#define ERROR_FREQ 200


// -- GYRO MODULE ------------------------------------------------------------
#define GYRO_DEFUSE_PIN 11

// GYRO FUNCTIONS :
void check_gyro_defuse_state(void);


// -- WIN VARIABLES ------------------------------------------------------------
uint8_t joystick_is_defused = 0;
uint8_t lcd_is_defused = 0;
uint8_t gyro_is_defused = 0;
uint8_t wire_is_defused = 0;



void setup() {
  Serial.begin(BAUD_RATE);
  randomSeed(analogRead(RAND_PIN));

  if (DEBUG) {
    Serial.println("\n-- DEFUSE INFORMATION ---------------------------------");
  }

  // -- LCD --------------------------------------------------------------------
  lcd.begin(LCD_WIDTH, LCD_HEIGHT);
  pinMode(UP_BTN, INPUT_PULLUP);
  pinMode(DOWN_BTN, INPUT_PULLUP);
  pinMode(LEFT_BTN, INPUT_PULLUP);
  pinMode(RIGHT_BTN, INPUT_PULLUP);

  uint8_t selected_word_id = random(10);
  selected_word = words[selected_word_id];
  encrypted_word = encryption(selected_word, generate_caesar_offset());
  word_length = encrypted_word.length();
  center_offset = (16 - word_length) / 2;

  lcd.setCursor(center_offset, 0);
  lcd.print(encrypted_word);

  // -- TIMER ------------------------------------------------------------------
  display.begin(COMMON_CATHODE, DIGIT_NUMBER, DIGIT_PINS, SEGMENT_PINS);
  display.setBrightness(DISPLAY_BRIGHTNESS);

  // -- JOYSTICK ---------------------------------------------------------------
  strip.begin();
  strip.show();
  strip.setBrightness(MAX_BRIGHTNESS);
  selected_joystick_seq = random(0, 10);
  selected_joystick_number = decimal_numbers[random(sizeof(decimal_numbers) / sizeof(decimal_numbers[0]))];
  trig_rotation = random(0, 2);

  show_binary_on_screen(selected_joystick_number, trig_rotation, DELAYED_DISPLAY);


  // -- WIRE -------------------------------------------------------------------
  pinMode(RED_LED_PIN, OUTPUT);
  pinMode(GREEN_LED_PIN, OUTPUT);

  for (uint8_t pin_index = 0; pin_index < TERM_COUNT; pin_index++) {
    pinMode(TERMINAL_PINS[pin_index], INPUT_PULLUP);
  }

  // -- BUZZER -----------------------------------------------------------------
  pinMode(BUZZER_PIN, OUTPUT);


  // -- GYRO -------------------------------------------------------------------
  pinMode(GYRO_DEFUSE_PIN, INPUT);

  // -- DEBUG ------------------------------------------------------------------
  if (DEBUG) {
    Serial.print("LCD MODULE SOLUTION : ");
    Serial.println(selected_word);
    Serial.print("JOYSTICK SEQUENCE SOLUTION : ");

    for (uint8_t index = 0; index < 4; index++) {

      switch (BUILTIN_JOYSTICK_SEQ[selected_joystick_seq][index]) {
        case J_UP:
          Serial.print("UP, ");
          break;
        case J_DOWN:
          Serial.print("DOWN, ");
          break;
        case J_LEFT:
          Serial.print("LEFT, ");
          break;
        case J_RIGHT:
          Serial.print("RIGHT, ");
          break;
        default:
          Serial.print("\nERROR, RESET NEDDED \n");
          break;
      }
    }

    Serial.println("\n\n");
  }



  Serial.print("1 -> ");
  Serial.println(digitalRead(A_PIN));
  Serial.print("2 -> ");
  Serial.println(digitalRead(B_PIN));
  Serial.print("3 -> ");
  Serial.println(digitalRead(C_PIN));
  Serial.print("4 -> ");
  Serial.println(digitalRead(D_PIN));
  Serial.print("5 -> ");
  Serial.println(digitalRead(E_PIN));
  Serial.print("6 -> ");
  Serial.println(digitalRead(F_PIN));
  Serial.print("7 -> ");
  Serial.println(digitalRead(G_PIN));
  Serial.print("8 -> ");
  Serial.println(digitalRead(H_PIN));
}

void loop() {
  // TIMER :
  decrement_time();

  // LCD :
  detect_button_press();
  lcd.setCursor(center_offset + user_cursor, 1);
  lcd.cursor();
  check_encryption();

  // JOYSTICK :
  event_joystick_listener();
  check_win();

  // WIRE :
  wire_disconnect_handler();
  check_wire_connection();

  // GYRO :
  check_gyro_defuse_state();

  // DEFUSE :
  check_defused_state();
}


// -- TIMER FUNCTIONS ----------------------------------------------------------

void decrement_time(void) {
  /*
  Function that update the seven segment display to the current ellapsed time. 
  */

  // Calculate how much time left according to the elapsed time.
  static uint8_t time_is_finished = 0;
  static uint16_t last_second_value = 0;
  uint32_t elapsed_time = millis() - timer_start;
  uint16_t total_elapsed_seconds = (START_TIME * 60) - (elapsed_time / 1000);

  if (time_is_finished) {
    time_blink();
    return;
  }

  if (SOUND_ON_TIMER_TICK && last_second_value != total_elapsed_seconds) {
    tone(BUZZER_PIN, TICK_FREQ, 25);
    last_second_value = total_elapsed_seconds;
  }

  // Calculate elapsed minutes and seconds from total_elapsed_seconds
  uint8_t minutes = total_elapsed_seconds / 60;
  uint8_t seconds = total_elapsed_seconds % 60;

  // Format the elapsed time, add zeros in front of minutes and seconds smaller than 9.
  if (minutes < 10 && seconds < 10) {
    sprintf(formatted_time, "0%d.0%d", minutes, seconds);
  } else if (minutes < 10) {
    sprintf(formatted_time, "0%d.%d", minutes, seconds);
  } else if (seconds < 10) {
    sprintf(formatted_time, "%d.0%d", minutes, seconds);
  } else {
    sprintf(formatted_time, "%d.%d", minutes, seconds);
  }

  // Check if the time is finished.
  if (total_elapsed_seconds <= 0) {

    // Time finished.
    time_is_finished = 1;
  }

  // display and refresh.
  display.setChars(formatted_time);
  display.refreshDisplay();
}


void time_blink(void) {
  /*
  Function that create the animation when the time is finished. 
  */
  uint16_t current_time = millis() / 1000;

  if (current_time % 2 == 0) {
    display.setChars("  .  ");
    display.refreshDisplay();
    return;
  }

  display.setChars(formatted_time);
  display.refreshDisplay();
}


// -- DEFUSE -------------------------------------------------------------------
void check_defused_state(void) {
  /*
  Function that check if every module has been defused.  
  */

  if (joystick_is_defused && lcd_is_defused && wire_is_defused && gyro_is_defused) {
    // Sound animation on win.
    if (SOUND_ON_EVENT) {
    }

    for (;;) {
      time_blink();
    }
  }
}


// -- LCD FUNCTIONS ------------------------------------------------------------
uint8_t generate_caesar_offset(void) {
  /*
  Function that generate 2 random offsets and print it on the screen according to special characters.  
  */
  uint8_t first_offset = random(5, 26);
  uint8_t second_offset = random(1, first_offset - 1);

  print_offset_glyph(first_offset, second_offset);
  return first_offset - second_offset;
}


void print_offset_glyph(uint8_t first_offset, uint8_t second_offset) {
  if (DEBUG) {
    Serial.print("FIRST_GLYPH_INDEX = ");
    Serial.println(first_offset);
    Serial.print("SECOND_GLYPH_INDEX = ");
    Serial.println(second_offset);
    Serial.print("TOTAL CRYPT OFFSET = ");
    Serial.println(first_offset - second_offset);
  }

  lcd.createChar(0, offset_glyph[first_offset]);
  lcd.createChar(1, offset_glyph[second_offset]);

  lcd.setCursor(0, 0);
  lcd.write(byte(0));

  lcd.setCursor(LCD_WIDTH - 1, 0);
  lcd.write(byte(1));
}

void detect_button_press(void) {
  /*
  Function that handle button press detection and actions. 
  */

  if (lcd_is_defused) {
    return;
  }

  if (digitalRead(UP_BTN) == 0) {
    // Up button pressed, cycle up letters.
    if (SOUND_ON_ACTION) { tone(BUZZER_PIN, ACTION_FREQ, 50); }
    update_char_value(ADD);
    wait_btn_release(UP_BTN);
  }

  if (digitalRead(DOWN_BTN) == 0) {
    // Down button pressed, cycle down letters.
    if (SOUND_ON_ACTION) { tone(BUZZER_PIN, ACTION_FREQ, 50); }
    update_char_value(SUBSTRACT);
    wait_btn_release(DOWN_BTN);
  }

  if (digitalRead(LEFT_BTN) == 0) {
    // Left button pressed, move the cursor to the left.
    if (SOUND_ON_ACTION) { tone(BUZZER_PIN, ACTION_FREQ, 50); }
    wait_btn_release(LEFT_BTN);
    user_cursor--;

    // Check if the cursor is out of bound.
    if (user_cursor < 0) {
      user_cursor = word_length - 1;
    }
  }

  if (digitalRead(RIGHT_BTN) == 0) {
    // Right button pressed, move the cursor to the right.
    if (SOUND_ON_ACTION) { tone(BUZZER_PIN, ACTION_FREQ, 50); }
    wait_btn_release(RIGHT_BTN);
    user_cursor++;

    // Check if the cursor is out of bound.
    if (user_cursor >= word_length) {
      user_cursor = 0;
    }
  }
}


void wait_btn_release(uint8_t button) {
  /*
  Function that wait the button to be released and execute action that not depend
  on which button is pressed. 
  */
  while (digitalRead(button) == 0) {
    decrement_time();
  }

  // Execute actions.
  print_input_word();
}


void print_input_word(void) {
  /*
  Function that print letter that the user selected. 
  */
  for (uint8_t index = 0; index < word_length; index++) {
    lcd.setCursor(center_offset + index, 1);

    if (input_word[index] < 65 || input_word[index] > 90) {
      // If the character is not a letter, print a blank character.
      lcd.write(' ');
      continue;
    }

    // Print the character.
    lcd.write(input_word[index]);
  }
}


void update_char_value(uint8_t action) {
  /*
  Function that increment or decrement the character value at a specific index. 
  */

  if (input_word[user_cursor] < 65 || input_word[user_cursor] > 90) {
    // If character is not initialised, initialise it.
    input_word[user_cursor] = (action == ADD) ? 65 : 90;
    return;
  }

  if (action == ADD) {
    // Add one to the current selected character.
    input_word[user_cursor]++;

    // Check if the character value is out of bound.
    if (input_word[user_cursor] > 90) {
      input_word[user_cursor] = 65;
    }

    return;
  }

  // Substract one to the current selected character.
  input_word[user_cursor]--;

  // Check if the character value is out of bound.
  if (input_word[user_cursor] < 65) {
    input_word[user_cursor] = 90;
  }

  return;
}


void check_encryption(void) {
  /*
  Function that check if the input sequence is correct.  
  */

  static uint8_t is_defused = 0;

  if (is_defused) {
    return;
  }

  for (uint8_t index = 0; index < word_length; index++) {
    // Check each character if one doesn't match, abort.
    if (selected_word[index] != input_word[index]) {
      return;
    }
  }

  lcd.noCursor();
  delay(500);
  lcd.setCursor(0, 0);
  lcd.print("    unlocked    ");

  if (SOUND_ON_EVENT) {
    // Sound animation on validation.

    uint8_t RHYTHM = 150;
    tone(BUZZER_PIN, 783, RHYTHM - 50);
    delay(RHYTHM);
    tone(BUZZER_PIN, 783, RHYTHM - 50);
    delay(RHYTHM);
    tone(BUZZER_PIN, 783, RHYTHM - 50);
    delay(RHYTHM);

    tone(BUZZER_PIN, 1046, RHYTHM);
    delay(RHYTHM * 2);
    tone(BUZZER_PIN, 783, RHYTHM - 50);
    delay(RHYTHM);
    tone(BUZZER_PIN, 1046, RHYTHM * 3);
    delay(RHYTHM * 3);
  }

  is_defused = 1;

  // Set win variable to true.
  lcd_is_defused = 1;
  if (DEBUG) { Serial.println("LCD HAS BEEN DEFUSED!!!"); }
}


String encryption(String word_to_encrypt, uint8_t gap) {
  /*
  Function that encrypt a word using cesar algorithm. 
  */

  uint8_t word_length = word_to_encrypt.length();
  char encrypt_word[word_length];

  // Add an offset to each letter of the word to encrypt.
  for (uint8_t index = 0; index < word_length; index++) {
    encrypt_word[index] = ((word_to_encrypt[index] - 'A' + gap) % 26) + 'A';
  }

  return encrypt_word;
}


// -- JOYSTICK FUNCTIONS -------------------------------------------------------
void check_win(void) {
  static uint8_t is_defused = 0;

  if (is_defused) {
    idle_animation();
    return;
  }

  if (joystick_seq_index > 3) {
    joystick_seq_index = 0;

    if (check_seq(selected_joystick_seq)) {
      blink_animation(3, 200, SUCCESS);
      idle_animation();

      is_defused = 1;
      joystick_is_defused = 1;

      if (DEBUG) { Serial.println("JOYSTICK HAS BEEN DEFUSED!!!"); }
      return;
    }

    blink_animation(3, 200, ERROR);
    show_binary_on_screen(selected_joystick_number, trig_rotation, INSTANT_DISPLAY);
    return;
  }
}


void show_binary_on_screen(int binary, int8_t trig_rotation, uint16_t effect_delay) {
  uint8_t offset = 0;
  uint32_t color = strip.Color(120, 0, 255);

  if (!trig_rotation) {
    offset = 14;
    color = strip.Color(0, 0, 255);
  }

  strip.setPixelColor(0, color);


  for (int bit_position = 0; bit_position < LED_COUNT - 1; bit_position++) {
    int bit_value = (decimal_numbers[selected_joystick_seq] >> bit_position) & 1;

    if (bit_value == 1) {
      strip.setPixelColor(abs(offset - bit_position) + 1, 255, 255, 255);
      delay(effect_delay);
    }

    strip.show();
  }
}


void blink_animation(uint8_t blink_count, uint16_t duration, int8_t effect_id) {
  display.setChars("    ");
  display.refreshDisplay();

  uint32_t color;
  uint16_t sound_freq = 0;

  switch (effect_id) {
    case -1:
      // ACTION.
      color = strip.Color(0, 0, 0);
      break;

    case 0:
      // SUCCESS.
      color = strip.Color(22, 218, 28);
      sound_freq = SUCCESS_FREQ;
      break;

    case 1:
      // ERROR.
      color = strip.Color(200, 0, 0);
      sound_freq = ERROR_FREQ;
      break;

    default:
      return;
  }

  for (uint8_t count = 0; count < blink_count; count++) {
    strip.clear();
    strip.show();
    if (SOUND_ON_EVENT) { tone(BUZZER_PIN, sound_freq, duration); }
    delay(duration);
    strip.fill(color, 0, LED_COUNT);
    strip.show();
    delay(duration);
  }

  strip.clear();
  strip.show();
}


void add_direction_to_seq(uint8_t direction) {
  blink_animation(1, ACTION_ANIM_DELAY, ACTION);
  show_binary_on_screen(selected_joystick_number, trig_rotation, INSTANT_DISPLAY);

  joystick_input_seq[joystick_seq_index] = direction;
  joystick_seq_index++;

  return;
}


bool check_seq(uint8_t seq_id) {
  for (uint8_t index = 0; index < 4; index++) {
    if (joystick_input_seq[index] != BUILTIN_JOYSTICK_SEQ[seq_id][index]) {
      // One direction does't match.
      if (DEBUG) {
        Serial.print(joystick_input_seq[index]);
        Serial.print(" != ");
        Serial.println(BUILTIN_JOYSTICK_SEQ[seq_id][index]);
      }

      return false;
    }
  }

  return true;
}


void event_joystick_listener() {
  /*
  Function that handle joystick direction event. 
  */

  if (joystick_is_defused) {
    return;
  }

  switch (joystick.facingDirection()) {
    case DIRECTION::UP:
      DEBUG ? Serial.print("UP, ") : Serial.print("");

      add_direction_to_seq(J_UP);
      wait_joystick_release();
      break;

    case DIRECTION::DOWN:
      DEBUG ? Serial.print("DOWN, ") : Serial.print("");

      add_direction_to_seq(J_DOWN);
      wait_joystick_release();
      break;

    case DIRECTION::LEFT:
      DEBUG ? Serial.print("LEFT, ") : Serial.print("");

      add_direction_to_seq(J_LEFT);
      wait_joystick_release();
      break;

    case DIRECTION::RIGHT:
      DEBUG ? Serial.print("RIGHT, ") : Serial.print("");

      add_direction_to_seq(J_RIGHT);
      wait_joystick_release();
      break;
  }
}

void wait_joystick_release() {
  /*
  Function that wait the joystick to be released.
  */

  if (SOUND_ON_ACTION) { tone(BUZZER_PIN, ACTION_FREQ, 50); }

  while (joystick.facingDirection() != DIRECTION::CENTER) {
    // Wait...
    decrement_time();
  }

  // Joystick released.
  return;
}

void idle_animation() {
  static uint8_t is_set = 0;

  if (is_set) {
    return;
  }

  // Set strip color to solid green.
  strip.fill(strip.Color(22, 218, 28), 0, LED_COUNT);
  strip.show();

  // Sound animation.
  if (SOUND_ON_EVENT) {
    uint8_t RHYTHM = 150;
    tone(BUZZER_PIN, 783, RHYTHM - 50);
    delay(RHYTHM);
    tone(BUZZER_PIN, 783, RHYTHM - 50);
    delay(RHYTHM);
    tone(BUZZER_PIN, 783, RHYTHM - 50);
    delay(RHYTHM);

    tone(BUZZER_PIN, 1046, RHYTHM);
    delay(RHYTHM * 2);
    tone(BUZZER_PIN, 783, RHYTHM - 50);
    delay(RHYTHM);
    tone(BUZZER_PIN, 1046, RHYTHM * 3);
    delay(RHYTHM * 3);
  }

  // No repeat.
  is_set = 1;
}


// -- WIRE FUNCTIONS -----------------------------------------------------------
void wire_disconnect_handler() {
  /*
  Function that update the wire connection in the table. 
  */

  // Check if the module is already defused.
  if (wire_is_defused) {
    return;
  }

  // Loop through each connection.
  for (uint8_t index = 0; index < TERM_COUNT; index++) {
    // Update the value.
    current_wire_state[index] = digitalRead(TERMINAL_PINS[index]);
  }
}


void check_wire_connection() {
  /*
  Function that check if correct wire were cut and set the win. 
  */

  static uint8_t is_defused = 0;

  // If the module is defused, activate green led.
  if (is_defused) {
    return;
  }

  // Else check if the connection are right.
  digitalWrite(RED_LED_PIN, HIGH);
  digitalWrite(GREEN_LED_PIN, LOW);

  // Loop through each connection.
  for (uint8_t index = 0; index < TERM_COUNT; index++) {
    if (current_wire_state[index] != BUILTIN_WIRE_STATES[index]) {
      return;
    }
  }

  // Every connection is correct.
  is_defused = 1;
  wire_is_defused = 1;
  digitalWrite(RED_LED_PIN, LOW);
  digitalWrite(GREEN_LED_PIN, HIGH);
  if (DEBUG) { Serial.println("WIRE HAS BEEN DEFUSED!!!"); }
}


// -- GYRO FUNCTIONS --------------------------------------------------------------
void check_gyro_defuse_state(void) {
  /*
  Function that check if the gyro module is defused. 
  */
  static uint8_t is_defused = 0;

  // Check if the module is already defused.
  if (is_defused) {
    return;
  }

  // Check the module state.
  if (digitalRead(GYRO_DEFUSE_PIN)) {
    if (DEBUG) { Serial.println("GYRO HAS BEEN DEFUSED!!!"); }

    if (SOUND_ON_EVENT) {
      // Sound animation on validation.

      uint8_t RHYTHM = 150;
      tone(BUZZER_PIN, 783, RHYTHM - 50);
      delay(RHYTHM);
      tone(BUZZER_PIN, 783, RHYTHM - 50);
      delay(RHYTHM);
      tone(BUZZER_PIN, 783, RHYTHM - 50);
      delay(RHYTHM);

      tone(BUZZER_PIN, 1046, RHYTHM);
      delay(RHYTHM * 2);
      tone(BUZZER_PIN, 783, RHYTHM - 50);
      delay(RHYTHM);
      tone(BUZZER_PIN, 1046, RHYTHM * 3);
      delay(RHYTHM * 3);
    }

    // State update.
    is_defused = 1;
    gyro_is_defused = 1;
  }
}