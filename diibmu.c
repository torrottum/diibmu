#include "math.h"
#include "raylib.h"

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

  InitWindow(screenWidth, screenHeight, "diibmu - visual timer");
  SetWindowMinSize(screenWidth / 2, screenHeight / 2);
  // todo: I guess we can go a whole lot lower
  // unless we want some fancy animations
  SetTargetFPS(60);

  Vector2 center = (Vector2){screenWidth / 2, screenHeight / 2};
  float minutesToDraw = 60.0f;
  float minutesSet = 30.0f;
  float elapsedMinutes = 0.0f;
  double startTime = 0.0f;
  TimerState timerState = SET;
  while (!WindowShouldClose()) {
    switch (timerState) {
    case SET: {
      // TODO: Swap this with IsKeyDown + debounce
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
        startTime = GetTime();
        timerState = COUNTDOWN;
        break;
      }
      if (minutesSet < 1)
        minutesSet = 1;
      if (minutesSet > 60)
        minutesSet = 60;
      minutesToDraw = minutesSet;
      break;
    }
    case COUNTDOWN: {
      elapsedMinutes = (float)(GetTime() - startTime) / 60.0f;
      minutesToDraw = minutesSet - elapsedMinutes;

      if (minutesToDraw <= 0) {
        minutesToDraw = 0;
        timerState = ALARM;
      }

      if (IsKeyPressed(KEY_P) || IsKeyPressed(KEY_SPACE)) {
        startTime = GetTime();
        timerState = PAUSE;
      }
      break;
    }
    case PAUSE: {
      if (IsKeyPressed(KEY_P) || IsKeyPressed(KEY_SPACE)) {
        startTime = GetTime() - elapsedMinutes * 60.0f;
        timerState = COUNTDOWN;
      }
      if (IsKeyPressed(KEY_X) || IsKeyPressed(KEY_DELETE) ||
          IsKeyPressed(KEY_BACKSPACE)) {
        timerState = SET;
      }
      break;
    }
    case ALARM: {
      // TODO: Play shound
      if (GetKeyPressed() != 0) {
        timerState = SET;
      }
      break;
    }
    default:
      break;
    }

    // TODO: draw dialog when ? is being held down

    BeginDrawing();
    ClearBackground(RAYWHITE);
    switch (timerState) {
    case PAUSE: {
      // TODO: draw as a centered dialog box or something
      DrawText("Paused", center.x - MeasureText("Paused", 40) / 2, 0, 40,
               BLACK);
      break;
    }
    case ALARM: {
      // TODO: draw as a centered dialog box or something
      DrawText("Time is up", center.x - MeasureText("Time is up", 40) / 2, 0,
               40, BLACK);
      break;
    }
    default:
      break;
    }
    DrawText("diibmu", 10, 25, 40, BLACK);
    DrawClock(center, 200, minutesToDraw, timerState);
    EndDrawing();
  }

  // deinit
  CloseWindow();
  return 0;
}
