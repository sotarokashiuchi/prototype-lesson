#include <Servo.h>
#include <math.h>

// ギヤの円ピッチ
#define X_CP (3 * 22)
#define Y_CP (3 * 32)

// 距離の分解能
#define DISTANCE_RESOLUTION 0.1

Servo servo_x;
Servo servo_y;

struct Position_t {
  double x;
  double y;
};

Position_t position_now = { 0, 0 };

void set_position(double x, double y, int delay_time) {
  if (!((0 <= x && x <= X_CP * M_PI * 0.5) && (0 <= y && y <= Y_CP * M_PI * 0.5))) {
    Serial.print("Error: larege size. ");
    Serial.print(x);
    Serial.print(" ");
    Serial.print(y);
    Serial.println("");
    return 0;
  }

  servo_x.writeMicroseconds((x * 1600 * 2) / (X_CP * M_PI) + 700);
  servo_y.writeMicroseconds((y * 1600 * 2) / (Y_CP * M_PI) + 700);
  position_now.x = x;
  position_now.y = y;

  Serial.print("x:");
  Serial.print((x * 1600 * 2) / (X_CP * M_PI) + 700);
  Serial.print(" y:");
  Serial.print((y * 1600 * 2) / (Y_CP * M_PI) + 700);
  Serial.println("");

  delay(delay_time);
}

void line(double dx, double dy) {
  double distance = sqrt(dx * dx + dy * dy);
  double count = distance / DISTANCE_RESOLUTION + 1;
  Position_t position_start = position_now;

  servo_x.detach();
  servo_y.detach();

  if (dx == 0 || dy == 0) {
    if (dx == 0) {
      set_position(position_start.x, position_start.y + dy, 0);
      servo_y.attach(5);
    }
    if (dy == 0) {
      set_position(position_start.x + dx, position_start.y, 0);
      servo_x.attach(3);
    }
    delay((abs(dx) + abs(dy)) * 10 + 1000);
  } else {
    for (int i = 0; i < count; i++) {
      set_position(
        position_start.x + dx / count * i,
        position_start.y + dy / count * i,
        0);
      servo_x.attach(3);
      servo_y.attach(5);
    }
    delay(300);
  }
}

void wait_any_key(void) {
  Serial.println("Enter any key");
  while (Serial.available() == 0);
  while (Serial.read() != -1);
}

void setup() {
  // 初期化処理
  Serial.begin(250000);
  servo_x.attach(3);
  servo_y.attach(5);
}

void loop() {
  // プロンプトの表示・選択
  Serial.println("");
  Serial.println("1:渦巻");
  Serial.println("2:直線");
  Serial.println("3:脱力");
  Serial.print(">>>");
  while (Serial.available() == 0);
  int mode = Serial.parseInt();
  while (Serial.read() != -1);
  Serial.println(mode);

  if (mode == 1) {
    // 渦巻モード
    double pitch = 0.8;
    set_position(2, 2, 1000);
    wait_any_key();

    for (double length = 96; 0 < length; length -= pitch * 2) {
      line(length, 0);
      line(0, length);
      line(-(length - pitch), 0);
      line(0, -(length - pitch));
    }
  }

  if (mode == 2) {
    // 直線モード
    set_position(100, 0, 5000);
    servo_x.detach();
    wait_any_key();
    line(0, 148);
  }

  if (mode == 3) {
    // 脱力モード
    servo_x.detach();
    servo_y.detach();
    wait_any_key();
    servo_x.attach(3);
    servo_y.attach(5);
  }
}
