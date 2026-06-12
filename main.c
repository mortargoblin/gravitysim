#include <stdio.h>
#include <math.h>
#include <stdbool.h>
#include <raylib.h>
#include <raymath.h>

#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 800

typedef struct {
  int size;
  float mass;
  Vector2 pos;
  Vector2 vel;
  Color color;
} Body;

void apply_gravity(Body *body1, Body *body2, float dt) {
    Vector2 dir = {
      body2->pos.x - body1->pos.x,
      body2->pos.y - body1->pos.y
    };

    float dist_sq = dir.x * dir.x + dir.y * dir.y;
    float dist = sqrtf(dist_sq);

    if (dist < 0.001f) return;

    dir.x /= dist;
    dir.y /= dist;
    
    float G = 50.0f; // mielivaltainen taikaluku :DD
    float accel_mag = G * body2->mass / dist_sq;
    Vector2 accel = {
      dir.x * accel_mag,
      dir.y * accel_mag
    };

    body1->vel.x += accel.x * dt;
    body1->vel.y += accel.y * dt;

    /*
    body1->pos.x += body1->vel.x * dt;
    body1->pos.y += body1->vel.y * dt;
    */
}

void draw_body(const Body *body) {
  DrawCircleV(body->pos, body->size, body->color);
}

void draw_velocity_line(Body *body) {
  float x = body->pos.x + body->vel.x;
  float y = body->pos.y + body->vel.y;
  DrawLine(body->pos.x, body->pos.y, x, y, WHITE);
}

int main(int argc, char **argv) {
  bool velocity_lines = false;

  if (argc > 1) {
    printf("ARGV 1: %s\n", argv[1]);
    if (*argv[1] == 'v') {
      velocity_lines = true;
    }
  }

  InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "gravity");
  SetTargetFPS(60);

  const Vector2 screen_center = {(float)SCREEN_WIDTH / 2, (float)SCREEN_HEIGHT / 2};
  Body bodies[] = {
    // size, mass, pos, vel, color
    {10, 10000, screen_center, {0, 0}, YELLOW}, // sun
    {4, 8, Vector2Subtract(screen_center, (Vector2){80, 0}), {0, 65}, RED},
    {5, 10, Vector2Subtract(screen_center, (Vector2){120, 0}), {0, 60}, GREEN},
    {7, 20, Vector2Subtract(screen_center, (Vector2){-150, 0}), {0, -50}, PURPLE},
    {9, 400, Vector2Subtract(screen_center, (Vector2){0, 180}), {-60, 0}, BLUE},
    {2, 3, Vector2Subtract(screen_center, (Vector2){20, 180}), {-60, 30}, MAGENTA},
  };
  int body_count = sizeof(bodies) / sizeof(bodies[0]);

  while (!WindowShouldClose()) {
    /// LOGIC
    float dt = GetFrameTime();
    
    for (int i = 0; i < body_count; i++) {
      for (int j = 0; j < body_count; j++) {
        apply_gravity(&bodies[i], &bodies[j], dt);
      }
    }

    for (int i = 0; i < body_count; i++) {
      bodies[i].pos.x += bodies[i].vel.x * dt;
      bodies[i].pos.y += bodies[i].vel.y * dt;
    }

    // INPUT
    static Vector2 last_mouse = {0};
    Vector2 mouse = GetMousePosition();

    if (IsMouseButtonDown(MOUSE_BUTTON_RIGHT)) {

      Vector2 delta = Vector2Subtract(mouse, last_mouse);

      for (int i = 0; i < body_count; i++) { // :DD
        bodies[i].pos = Vector2Add(bodies[i].pos, delta);
      }

    }
    last_mouse = mouse;

    if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
      printf("CLICKED\n");

      Body new_planet;
      new_planet.pos = GetMousePosition();
      new_planet.vel = (Vector2){-30, 0};
      new_planet.size = 10;
      new_planet.mass = 20;
      new_planet.color = WHITE;

      bodies[body_count] = new_planet;
      body_count++;
      printf("body_count: %d\n", body_count);
    }

    /// RENDERING
    BeginDrawing();
    {
      ClearBackground(BLACK);
      DrawText(
          TextFormat(
            "FRAME TIME\n%f\n\nEARTH VELOCITY\nX: %.2f\nY: %.2f",
            dt, bodies[1].vel.x, bodies[1].vel.y
            ),
          10, 10, 20, WHITE
          );

      for (int i = 0; i < body_count; i++) {
        draw_body(&bodies[i]);
      }

      if (velocity_lines) {
        for (int i = 0; i < body_count; i++) {
          draw_velocity_line(&bodies[i]);
        }
      }

    }
    EndDrawing();
  }

  CloseWindow();
  return 0;
}
