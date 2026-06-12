#include <stdio.h>
#include <math.h>
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

void draw_body(const Body *body) {
  DrawCircle(body->pos.x, body->pos.y, body->size, body->color);
}

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

int main() {
  printf("init\n");

  InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "gravity");
  SetTargetFPS(60);

  const Vector2 screen_center = {(float)SCREEN_WIDTH / 2, (float)SCREEN_HEIGHT / 2};
  Body bodies[] = {
    // size, mass, pos, vel, color
    {10, 10000, screen_center, {0, 0}, YELLOW}, // sun
    {4, 8, Vector2Subtract(screen_center, (Vector2){80, 0}), {0, 65}, RED},
    {5, 10, Vector2Subtract(screen_center, (Vector2){120, 0}), {0, 60}, GREEN},
    {7, 16, Vector2Subtract(screen_center, (Vector2){-150, 0}), {0, -50}, PURPLE},
    {9, 400, Vector2Subtract(screen_center, (Vector2){0, 180}), {-60, 0}, BLUE},
    {2, 3, Vector2Subtract(screen_center, (Vector2){20, 180}), {-60, 30}, MAGENTA},
  };

  while (!WindowShouldClose()) {
    /// LOGIC
    float dt = GetFrameTime();
    
    int body_count = sizeof(bodies) / sizeof(bodies[0]);
    for (int i = 0; i < body_count; i++) {
      for (int j = 0; j < body_count; j++) {
        apply_gravity(&bodies[i], &bodies[j], dt);
      }
    }

    for (int i = 0; i < body_count; i++) {
      bodies[i].pos.x += bodies[i].vel.x * dt;
      bodies[i].pos.y += bodies[i].vel.y * dt;
    }

    /// RENDERING
    BeginDrawing();
    {
      ClearBackground(BLACK);
      DrawText(
          TextFormat("EARTH VELOCITY\nX: %.2f\nY: %.2f",
            bodies[1].vel.x, bodies[1].vel.y),
          10, 10, 20, WHITE
          );

      for (int i = 0; i < body_count; i++) {
        draw_body(&bodies[i]);
      }
    }
    EndDrawing();
  }

  CloseWindow();
  return 0;
}
