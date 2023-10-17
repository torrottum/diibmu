#include "math.h"
#include "raylib.h"

#define MAX(a, b) ((a) > (b) ? (a) : (b))
#define MIN(a, b) ((a) < (b) ? (a) : (b))

typedef enum { SET, COUNTDOWN, ALARM, PAUSE } TimerState;

void DrawClock(Vector2 center, float radius, float minutes, TimerState state) {
  for (int i = 0; i < 60; i++) {
    float angle = (360.0f / 60.0f) * i;
    float startRadius = (i % 5 == 0) ? radius - 15 : radius - 10;
    float endRadius = (i % 5 == 0) ? radius + 15 : radius + 10;
    Color markColor = (i % 5 == 0) ? BLACK : GRAY;
    Vector2 start = {
        center.x + startRadius * sinf(angle * DEG2RAD),
        center.y + startRadius * cosf(angle * DEG2RAD),
    };
    Vector2 end = {
        center.x + endRadius * sinf(angle * DEG2RAD),
        center.y + endRadius * cosf(angle * DEG2RAD),
    };

    DrawLineV(start, end, markColor);

    if (i % 5 == 0) {
      int num = 60 - i == 60 ? 0 : 60 - i;
      Vector2 textPos = {center.x + sinf(angle * DEG2RAD) * (radius + 40),
                         center.y - cosf(angle * DEG2RAD) * (radius + 40)};

      DrawText(TextFormat("%d", num),
               textPos.x - MeasureText(TextFormat("%d", num), 20) / 2,
               textPos.y - 15, 30, BLACK);
    }
  }

  float endAngle = (minutes / 60.0f) * 360.0f;
  Color color;
  switch (state) {
  case SET: {
    color = MAROON;
    break;
  }
  case COUNTDOWN: {
    color = RED;
    break;
  }
  default: {
    color = BLACK;
    break;
  }
  }
  DrawCircleSector(center, radius, 180, 180 + endAngle, 72, color);
  DrawCircleV(center, 10, DARKGRAY);
}

int main(int argc, char *argv[]) {
  int screenWidth = 600;
  int screenHeight = 600;
  SetConfigFlags(FLAG_WINDOW_RESIZABLE | FLAG_VSYNC_HINT);
  InitWindow(600, 600, "diibmu - visual timer");
  SetWindowMinSize(600, 600);
  SetTargetFPS(60);

  Vector2 center = (Vector2){screenWidth / 2, screenHeight / 2};
  float minutesToDraw = 60.0f;
  float minutesSet = 60.0f;
  TimerState timerState = SET;
  while (!WindowShouldClose()) {
    switch (timerState) {
    case SET: {
      switch (GetKeyPressed()) {
      case KEY_J:
      case KEY_DOWN:
        minutesSet -= 1.0f;
        break;
      case KEY_K:
      case KEY_UP:
        minutesSet += 1.0f;
        break;
      case KEY_ENTER:
        timerState = COUNTDOWN;
        break;
      }

      if (minutesSet < 1) {
        minutesSet = 1;
      } else if (minutesSet > 60) {
        minutesSet = 60;
      }

      minutesToDraw = minutesSet;
      break;
    }
    case COUNTDOWN: {
      break;
    }
    case PAUSE: {
      break;
    }
    case ALARM: {
      break;
    }
    default:
      break;
    }

    BeginDrawing();
    ClearBackground(WHITE);
    DrawText("diibmu", 10, 25, 40, BLACK);
    DrawClock(center, 200, minutesToDraw, timerState);
    EndDrawing();
  }

  // deinit
  CloseWindow();
  return 0;
}
