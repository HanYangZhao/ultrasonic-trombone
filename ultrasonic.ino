#include <Bounce2.h>
#define PIN_MIC A8
#define MOUSE_LEFTCLICK_PIN 23
// SETTINGS
#define p_trigPin_cadangan  1
#define p_echoPin_cadangan  0
#define SCREEN_WIDTH 1920   // Doesn't have to be exact;
#define SCREEN_HEIGHT 1080  // probably just the ratio.
#define SLIDE_MAX 290
#define TOOT_THRESHOLD 200
#define DELAY 1
#define AVG_TOOT_COUNT 5
// GLOBALS
int toots[AVG_TOOT_COUNT]; // to hold the running average
int mouse_y = 0;
int avg_dist_mm = 0;
float dist_cm = 0;
Bounce2::Button mouse_button = Bounce2::Button();
int initial_mic_level = 0;
void us_setup()
{
  pinMode(p_trigPin_cadangan, OUTPUT);
  pinMode(p_echoPin_cadangan, INPUT);
}
float us_get_jarak(int triger, int echo)
{
  long duration;
  float distance;
  int i;

  digitalWrite(triger, LOW);
  delayMicroseconds(2);
  digitalWrite(triger, HIGH);
  delayMicroseconds(10);
  digitalWrite(triger, LOW);
  duration =  pulseIn(echo, HIGH,10000); //uS

  distance = duration * 0.034 / 2; //mm

  return distance;
}

//--------------------------------#MY AVERAGE
#define n_my_data 10
float my_data[n_my_data];
int i_my_data = 0;

float my_moving_average(float in_data, int debug)
{
  float my_ave;
  float my_sum;

  my_data[i_my_data] = in_data;

  if (my_data[n_my_data - 1] == 0)
  {
    my_sum = 0;

    for (int i = 0; i <= i_my_data; i++)
    {
      my_sum += my_data[i];
    }
    my_ave = my_sum / (i_my_data + 1);
  }
  else
  {
    my_sum = 0;
    for (int i = 0; i < n_my_data; i++)
    {
      my_sum += my_data[i];
    }
    my_ave = my_sum / n_my_data;
  }


  //#Debug
  if (debug)
  {
    for (int i = 0; i < n_my_data; i++)
    {
      Serial.print(i); Serial.print(" "); Serial.println(my_data[i]);
    }
    Serial.print("i"); Serial.print(" "); Serial.println(i_my_data);
    Serial.print("Last"); Serial.print(" "); Serial.println(my_data[n_my_data - 1]);

    Serial.print("Sum"); Serial.print(" "); Serial.println(my_sum);
    Serial.print("Ave"); Serial.print(" "); Serial.println(my_ave);
    Serial.println();
  }



  i_my_data++;
  if (i_my_data >= n_my_data)
    i_my_data = 0;

  return my_ave;
}

void my_moving_ave_clear()
{
  //Make sure All Array is 0
  for (int i = 0; i < n_my_data; i++)
  {
    my_data[i] = 0;
  }
}

int smoothJazz(int toot) {
  static int averageToot = 0;
  static int readIndex = 0;              // the index of the current reading
  static int total = 0;                  // the running total
  // subtract the last reading:
  total = total - toots[readIndex];
  // read from the sensor:
  toots[readIndex] = toot;
  // add the reading to the total:
  total = total + toots[readIndex];
  // advance to the next position in the array:
  readIndex = readIndex + 1;
  // if we're at the end of the array...
  if (readIndex >= AVG_TOOT_COUNT) {
    // ...wrap around to the beginning:
    readIndex = 0;
  }
  // calculate the average:
  return total / AVG_TOOT_COUNT;
}

void turdAverage() {
  for (int r = 0; r < TOOT_THRESHOLD; r++) {
    toots[r] = 0;
  }
}

void tootTromboneBlow(int toot) {
  static bool blowing = false;
  if (toot > TOOT_THRESHOLD) {
    if (!blowing) {
      Mouse.set_buttons(1, 0, 0);
    }
    blowing = true;
  } else {
    if (blowing) {
      Mouse.set_buttons(0, 0, 0);
      turdAverage();
    }
    blowing = false;
  }
}

void tootTromboneClick() {
  mouse_button.update(); // Update the Bounce instance
  if (mouse_button.isPressed()){
    Mouse.set_buttons(1, 0, 0);
  } else {
    Mouse.set_buttons(0, 0, 0);
  }
}

void setup() {
  Serial.begin(9600);
  Mouse.screenSize(SCREEN_WIDTH, SCREEN_HEIGHT);
  us_setup();
  my_moving_ave_clear();
  mouse_button.attach(MOUSE_LEFTCLICK_PIN, INPUT_PULLUP);
  mouse_button.interval(10);
  mouse_button.setPressedState(LOW);

}

void loop()
{
  tootTromboneClick();
//  tootTromboneBlow(smoothJazz(analogRead(PIN_MIC)));
  initial_mic_level = analogRead(PIN_MIC);
  Serial.println(smoothJazz(analogRead(PIN_MIC) - initial_mic_level));
  dist_cm =  us_get_jarak(p_trigPin_cadangan, p_echoPin_cadangan);
  avg_dist_mm = my_moving_average(dist_cm, 0) * 10;
  mouse_y = (int)((float)SCREEN_HEIGHT * ((float)avg_dist_mm/(float)SLIDE_MAX)) - 0;
  if (mouse_y > SCREEN_HEIGHT){
    mouse_y = SCREEN_HEIGHT;
  } else if (mouse_y < 0) {
    mouse_y = 0;
  }
//  Serial.println(avg_dist_mm); 
  Mouse.moveTo(500, mouse_y);
  delay(5);
}
